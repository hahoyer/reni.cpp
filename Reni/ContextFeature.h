#pragma once
#include "Feature.h"

using namespace HWLib;

namespace Reni
{
    class AccessCache;
    class Syntax;
    class TokenClass;

    class ContextFeature final
        : public WithId<DumpableObject, Feature>
    {
        typedef DumpableObject baseType;
        typedef ContextFeature thisType;
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
        Ref<AccessCache> resultCache;

        ContextFeature(Simple const& simple, Extended const& extended);

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