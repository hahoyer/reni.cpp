#pragma once

#include "Properties.h"

namespace HWLib
{
    using Chars = char const*;

    struct Type
    {
        Type(type_info const& info) : info(info){};
        p(Chars, Name){ return info.name(); }
    private:
        type_info const& info;
    };

#ifdef xx
    class CMetaDataItem
    {
        OptRef<DumpableObject> boxedValue;
    public:
        Type const Type;
        String const Name;
        Ref<DumpableObject> const& Value;

        CMetaDataItem(type const& type, str const& name, any const& value)
            : Name(name)
            , Type(type)
            , Value(value)
            , _anyBoxedValue(empty)
        {
        };

        CMetaDataItem(type const& type, str const& name, str const& value)
            : Name(name)
            , Type(type)
            , Value(*new anyBoxed<str>(value))
        {
            _anyBoxedValue = dynamic_cast<CCountedRef const&>(Value);
        };

        CMetaDataItem(type const& type, str const& name, __int64 value)
            : Name(name)
            , Type(type)
            , Value(*new anyBoxed<__int64>(value))
        {
            _anyBoxedValue = dynamic_cast<CCountedRef const&>(Value);
        };

        CMetaDataItem(type const& type, str const& name, any const* value)
            : Name(name)
            , Type(type)
            , Value(*new anyBoxed<any const*>(value))
        {
            _anyBoxedValue = dynamic_cast<CCountedRef const&>(Value);
        };
    };


    class Object : public CCountedRef
    {
        static int NextId;
        int const _objectId;

    protected:
        Object(int NextId);
        Object();

    public:
        GETTER(int, ObjectId){ return _objectId; };

        virtual str const shortdump()const;
        str const dump()const{ return sdump(); };
        str const keydump()const{ return vardump(ObjectId) + any::dump(); };
        str const dumpheader()const;
        str const dumpdata()const;

        virtual int const cutlength()const{ return 100; };
        virtual str const cutteddump()const;

        VIRTUAL_GETTER(TArray<CMetaDataItem>, MetaDataItems){ return empty; };
    };
#endif
};

#define METADATA_FUNCTIONHEADER virtual HWLib::TArray<Common::CMetaDataItem> const get_MetaDataItems()const
#define METADATA_FUNCTION(Class) HWLib::TArray<Common::CMetaDataItem> const Class::get_MetaDataItems()const
#define BEGIN_METADATA_IMPLEMENTATION {return HWLib::TArray<Common::CMetaDataItem>(inherited::get_MetaDataItems())
#define BEGIN_METADATA(Class) METADATA_FUNCTION(Class) BEGIN_METADATA_IMPLEMENTATION 
#define BEGIN_METADATAHEADER METADATA_FUNCTIONHEADER BEGIN_METADATA_IMPLEMENTATION 
#define METADATA(x) (Common::CMetaDataItem(typeof(*this),quine(x)))
#define END_METADATA ;}
#define METADATA1(x) BEGIN_METADATAHEADER METADATA(x) END_METADATA
#define METADATA2(x0,x1) BEGIN_METADATAHEADER METADATA(x0) METADATA(x1) END_METADATA
#define METADATA3(x0,x1,x2) BEGIN_METADATAHEADER METADATA(x0) METADATA(x1) METADATA(x2) END_METADATA
#define METADATA4(x0,x1,x2,x3) BEGIN_METADATAHEADER METADATA(x0) METADATA(x1) METADATA(x2) METADATA(x3) END_METADATA
#define METADATA5(x0,x1,x2,x3,x4) BEGIN_METADATAHEADER METADATA(x0) METADATA(x1) METADATA(x2) METADATA(x3) METADATA(x4) END_METADATA

