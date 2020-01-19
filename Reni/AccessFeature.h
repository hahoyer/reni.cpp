#pragma once
#include "Feature.h"

using namespace HWLib;

namespace Reni
{
    class AccessCache;
    class AccessFeature;
    class ContainerContext;
    class ExtendedFeature;
    class SimpleFeature;
    class Syntax;
    class TokenClass;
    template<> class FoundFeature < AccessFeature > ;

    class AccessFeature final
        : public WithId < DumpableObject, AccessFeature >
    {
        typedef DumpableObject baseType;
        typedef AccessFeature thisType;
    public:

        class Simple : public RefCountProvider
        {
            using thisType = Simple;
        public:
            ThisRef;
            virtual ResultData const Result(Context const&target, Category category) const = 0;
        };

        class Extended : public RefCountProvider
        {
            using thisType = Extended;
        public:
            ThisRef;
            virtual ResultData const Result(Context const&target, Category category, Type const&arg) const = 0;
        };

        Optional<Ref<Simple>> const simple;
        Optional<Ref<Extended>> const extended;

        AccessFeature(Simple const& simple, Extended const& extended);

        ThisRef;

        ResultData const FunctionResult(
            Context const&context,
            Category category,
            Optional<Ref<Syntax>> const& right
            )const;
        p(bool, isEmpty){ return simple.IsEmpty && extended.IsEmpty; }
    private:
        p_function(Array<String>, DumpData) override{ return{nd(simple) + nd(extended)}; }
    };

    template<>
    class FoundFeature<AccessFeature> final : public FoundFeatureBase<AccessFeature>
    {
        using baseType = FoundFeatureBase < AccessFeature > ;
        using thisType = FoundFeature;

        AccessFeature const feature;
    public:
        FoundFeature(AccessFeature const&feature)
            : feature(feature)
        {
            SetDumpString();
        }

        ResultData const FunctionResult(
            Context const&context,
            Category category,
            Optional<Ref<Syntax>> const& right
            )const;
    private:
        p_function(Array<String>, DumpData) override{ return{nd(feature)}; };
    };
}

#include "ResultData.h"

using namespace Reni;

inline ResultData const FoundFeature<AccessFeature>::FunctionResult(
    Context const& context,
    Category category,
    Optional<Ref<Syntax>> const& right
)const
{
    return feature.FunctionResult(context, category, right);
};
