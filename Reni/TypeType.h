#pragma once
#include "Feature.h"
#include "Type.h"
#include "../Util/Size.h"

class InstanceFunctionFeature;
using namespace HWLib;
using namespace Util;


namespace Reni
{
    class InstanceToken;
    template<class T>
    class SearchResult;

    class TypeType final : public Type
    {
        typedef Type baseType;
        typedef TypeType thisType;
    public:
        WeakRef<Type> value;
        TypeType(Type const& value);
        HW_PR_THISREF;

        template<class TTokenClass>
        SearchResult<Feature> DeclarationsForType() const;
        static Optional<WeakRef<thisType>> const Convert(Type const&target);
    private:
        HW_PR_DECL_GETTER(Array<string>,DumpData) override{return{ HW_D_VALUE(*value) };};
        HW_PR_DECL_GETTER(bool, hollow) { return true; };
        HW_PR_DECL_GETTER(Size, size) override{ return 0; }
        HW_PR_DECL_GETTER(WeakRef<Global>, global) override{return value->global;}
        SearchResult<Feature> DeclarationsForType(DeclarationType const& token) const override;
    };
};

using namespace Reni;

template<>
inline SearchResult<Feature> TypeType::DeclarationsForType<InstanceToken>() const
{
    return Feature::From<InstanceFunctionFeature>(*this);
}

