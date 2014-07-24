#pragma once
#include "Type.h"

using namespace HWLib;

namespace Reni
{
    class AccessData;
    class ContainerContext;
    class ColonEqual;

    class AccessType final : public Type
    {
        using baseType = Type;
        using thisType = AccessType;

        WeakRef<AccessData> data;
    public:
        AccessType(AccessType const&) = delete;
        explicit AccessType(AccessData const& data);
        ThisRef;

        template<class TTokenClass>
        SearchResult<Feature> const DeclarationsForType()const;
        static Optional<WeakRef<thisType>> const Convert(Type const&target);
    private:
        p(WeakRef<Type>, value);
        p_function(bool, hllw) { return false; };
        p_function(Array<String>, DumpData) override;
        p_function(Size, size)override { return Size::Address; }
        p_function(WeakRef<Global>, global) override;
        Optional<WeakRef<NumberType>> const get_asNumberType() const override;
        SearchResult<Feature> const DeclarationsForType(DeclarationType const& token) const override;
        Ref<ResultCache> const DirectConvert() const override;
        Ref<CodeItem> const DirectConvertCode() const;

        class AssignmentFeature final : public Feature::Extended
        {
            using baseType = Feature::Extended;
            using thisType = AssignmentFeature;

            ResultData const Result(Category category, Type const& target, Type const& arg) const override;
        };
    };

};


#include "DumpPrintToken.h"

using namespace Reni;

template <>
inline SearchResult<Feature> const AccessType::DeclarationsForType<ColonEqual>() const
{
    return Feature::From<AssignmentFeature>(*this);
}

template <class TTokenClass>
inline SearchResult<Feature> const AccessType::DeclarationsForType() const
{
    return Feature::Error(Dump + "\n" + typeid(TTokenClass).name());
};
