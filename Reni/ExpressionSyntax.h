#pragma once
#include "Syntax.h"
using namespace HWLib;

#include "Context.h"
#include "SearchResult.h"

namespace Reni
{
    class ExpressionSyntax : public Syntax
    {
        using baseType = Syntax;
        using thisType = ExpressionSyntax;
        TokenClass const& tokenClass;
        Ref<Syntax, true> const left;
        Ref<Syntax, true> const right;
    public:
        ExpressionSyntax(TokenClass const& tokenClass, Ref<Syntax, true > const left, SourcePart const part, Ref<Syntax, true > const right);
    private:
        override_p_function(Array<String>, DumpData);
        virtual ResultData const GetResultData(Context const&context, Category category)const override;
    };
}