#pragma once
#include "Feature.h"

using namespace HWLib;

namespace Reni
{
    class AccessCache;
    class ContainerContext;
    class ExtendedFeature;
    class SimpleFeature;
    class Syntax;
    class TokenClass;

    class AccessFeature final
        : public WithId<DumpableObject, Feature>
    {
        typedef DumpableObject baseType;
        typedef AccessFeature thisType;
    public:

        class Simple : public RefCountProvider
        {
            using baseType = DumpableObject;
            using thisType = Simple;
        public:
            ThisRef;
            virtual ResultData const Result(Context const&target, Category category) const = 0;
        };

        class Extended : public RefCountProvider
        {
            using baseType = DumpableObject;
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
        p_function(Array<String>,DumpData) override{return{nd(simple)+nd(extended)};}
    };
}