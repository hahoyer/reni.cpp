#pragma once
#include "../HWLib/RefCountProvider.h"
#include "../HWLib/DumpableObject.h"
#include "../Util/Category.h"
#include "../HWLib/Ref.h"
#include "../HWLib/Optional.h"
#include "../HWLib/WeakRef.h"

#include "SearchResult.h"

using namespace HWLib;
using namespace Util;

namespace Reni
{
    class ArrayType;
    class Context;
    class Feature;
    class Syntax;
    class ResultData;
    class ResultFromSyntaxAndContext;
    class Type;
    template<> class FoundFeature < Feature > ;

    class Feature final
        : public WithId < DumpableObject, Feature >
    {
        using baseType = DumpableObject;
        using thisType = Feature;
    public:
        template<class T>
        static FoundFeature<Feature> From(Type const&type);
        static FoundFeature<Feature> Error(string const& title);

        class Simple : public RefCountProvider
        {
            using thisType = Simple;
        public:
            HW_PR_THISREF;
            virtual ResultData Result(Category const& category, Type const&target) const = 0;
        };


        class Extended : public RefCountProvider
        {
            using thisType = Extended;
        public:
            HW_PR_THISREF;
            virtual ResultData Result(Category const& category, Type const&target, Type const&arg) const = 0;
        };


        Optional<Ref<Simple>> const simple;
        Optional<Ref<Extended>> const extended;

    private:
        Feature(){};

        explicit Feature(Extended const& extended)
            : extended(extended.thisRef)
        {
        }

        explicit Feature(Simple const& simple)
            : simple(simple.thisRef)
        {
        }

    public:
        HW_PR_GET(bool, isEmpty){ return simple.IsEmpty && extended.IsEmpty; }
        ResultData ConversionResult(const Category& category, Type const& target, Type const& destination) const;
    private:
        HW_PR_DECL_GETTER(Array<string>, DumpData) override{ return{HW_D_VALUE(simple) + HW_D_VALUE(extended)}; }
    };


    class EnableCutFeature final : public Feature::Simple
    {
        using baseType = Simple;
        using thisType = EnableCutFeature;
        ResultData Result(Category const& category, Type const& target) const override;
    };

    class DumpPrintFeature final : public Feature::Simple
    {
        using baseType = Simple;
        using thisType = DumpPrintFeature;
        ResultData Result(Category const& category, Type const& target) const override;
    };

    template<>
    class FoundFeature<Feature> final : public FoundFeatureBase<Feature>
    {
        using baseType = FoundFeatureBase<Feature>;
        using thisType = FoundFeature;

        WeakRef<Type> const type;
        Feature const feature;
        Array<WeakRef<Type>> const path;

        FoundFeature(Type const& type, Feature const& feature, Array<WeakRef<Type>> const& path);
    public:
        FoundFeature(Type const& type, Feature const& feature);
        FoundFeature(FoundFeature const&);;

        ResultData AlignThis(ResultData const& start) const;
        ResultData FunctionResult(
          Context const& context,
          const Category& category,
          Optional<Ref<Syntax>> const& left,
          Optional<Ref<Syntax>> const& right
        ) const;
        thisType operator+(Type const& fromType) const;

        ResultData ConversionResult(const Category& category, Type const& target, Type const& destination) const;

    private:
        HW_PR_DECL_GETTER(Array<string>, DumpData) override{ return{HW_D_VALUE(type) + HW_D_VALUE(feature) + HW_D_VALUE(path)}; };
        ResultData Result(Context const& context, const Category& category, Type const& target,
                          Optional<Ref<Syntax>> const& arg) const;
    };

}

using namespace Reni;

template <class T>
FoundFeature<Feature> Feature::From(Type const&type)
{
    return FoundFeature<Feature>(type, Feature(*new T));
}

