#pragma once
#include "ContextFeatureProvider.h"
#include "DumpPrintToken.h"

using namespace HWLib;

namespace Reni{
    class SignFeatureProvider final : public ContextFeatureProvider<SignToken>{
        typedef FeatureProvider<SignToken> baseType;
        typedef SignFeatureProvider thisType;

        class Feature final : public ContextFeature{
            typedef ContextFeature baseType;
            typedef Feature thisType;
        private:
            virtual ResultData const FunctionResult(
                Context const&context,
                Category category,
                TokenClass const&tokenClass,
                Ref<Syntax, true> const& right
                )const override;
            override_p_function(Array<String>, DumpData) { return{}; }
        };

        override_p_function(Ref<ContextFeature>, feature){ return new Feature(); }
        override_p_function(Array<String>, DumpData) { return{}; }
    };


}
