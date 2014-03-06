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
        CtrlPtr<Syntax > const left;
        CtrlPtr<Syntax > const right;
    public:
        ExpressionSyntax(TokenClass const& tokenClass, CtrlPtr<Syntax > const left, SourcePart const part, CtrlPtr<Syntax > const right);
    private:
        override_p_function(Array<String>, DumpData);
        virtual ResultData const GetResultData(Context const&context, Category category)const override;
    };
}