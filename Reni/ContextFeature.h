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
            Ref<Syntax, true> const& right
            )const;
    private:
        virtual ResultData const FunctionResult(
            Context const&context,
            Category category,
            ExpressionSyntax const& expressionSyntax
            )const override final;
        p_function(Array<String>,DumpData) override {
            return{};
        }
    };

}
