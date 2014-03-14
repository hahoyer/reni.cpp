#pragma once
#include "Feature.h"

using namespace HWLib;

namespace Reni{
    class Syntax;

    class ContextFeature
        : public Feature{
        typedef Feature baseType;
        typedef ContextFeature thisType;

        virtual ResultData const FunctionResult(
            Context const&context,
            Category category,
            ExpressionSyntax const& expressionSyntax
            )const override final;
        
        virtual ResultData const FunctionResult(
            Context const&context,
            Category category,
            Ref<Syntax,true> const& right
            )const;

        override_p_function(Array<String>, DumpData) {
            return{};
        }
    };

}
