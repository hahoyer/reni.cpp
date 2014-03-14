#pragma once
#include "Feature.h"

using namespace HWLib;

namespace Reni{
    class Syntax;
    class TokenClass;

    class ContextFeature
        : public Feature{
        typedef Feature baseType;
        typedef ContextFeature thisType;
    protected:
        virtual ResultData const FunctionResult(
            Context const&context,
            Category category,
            TokenClass const&tokenClass,
            Ref<Syntax, true> const& right
            )const;
    private:
        virtual ResultData const FunctionResult(
            Context const&context,
            Category category,
            ExpressionSyntax const& expressionSyntax
            )const override final;
        override_p_function(Array<String>, DumpData) {
            return{};
        }
    };

}
