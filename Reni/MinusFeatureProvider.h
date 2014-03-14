#pragma once
#include "ContextFeatureProvider.h"
#include "DumpPrintToken.h"

using namespace HWLib;

namespace Reni{
    class MinusFeatureProvider final : public ContextFeatureProvider<MinusToken>{
        typedef FeatureProvider<MinusToken> baseType;
        typedef MinusFeatureProvider thisType;

        class Feature final : public ContextFeature{
            typedef ContextFeature baseType;
            typedef Feature thisType;
        private:
            virtual ResultData const FunctionResult(
                Context const&context,
                Category category,
                Ref<Syntax, true> const& right
                )const override;
            override_p_function(Array<String>, DumpData) { return{}; }
        };
    public:
        AssumeConstObject;
    private:
        override_p_function(Ref<ContextFeature>, feature){ return new Feature(); }
        override_p_function(Array<String>, DumpData) { return{}; }
    };


}
