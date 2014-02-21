#pragma once

#include "Properties.h"
#include <string>

namespace HWLib
{
    class String;

    class DumpableObjectBase
    {
        mutable std::string dumpString;
        mutable std::string dumpShortString;
    public:
        virtual_p(String, DumpLong) = 0;
        virtual_p(String, DumpShort);
        p(String, Dump);
    protected:
        DumpableObjectBase();
        virtual ~DumpableObjectBase(){};
        void SetDumpString();
    };

    template<typename T> class Array;

    template<typename TBase, typename TRealm = TBase>
    class WithId: public TBase{
        using baseType = TBase;
        using thisType = WithId;
    public:
        static int NextObjectId;
        int const ObjectId;
        WithId() : ObjectId(NextObjectId++){};
        override_p_function(String, DumpHeader);
    };

    class DumpableObject : public DumpableObjectBase
    {
        using baseType = DumpableObjectBase;
        using thisType = DumpableObject;
    protected:
        DumpableObject();
        virtual_p(Array<String>, DumpData) = 0;
        virtual_p(String, DumpHeader);
    public:
        override_p_function(String, DumpShort);
        p(bool, IsInDump){ return isInDump; }
        override_p_function(String, DumpLong)final;
    private:
        mutable bool isInDump;
    };
};


template<typename TBase, typename TRealm>
int HWLib::WithId<TBase, TRealm>::NextObjectId = 0;

