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
        OptRef<Syntax > const left;
        OptRef<Syntax > const right;
    public:
        ExpressionSyntax(TokenClass const& tokenClass, OptRef<Syntax > const left, SourcePart const part, OptRef<Syntax > const right);
    private:
        override_p_function(Array<String>, DumpData);
        virtual ResultData const GetResultData(Context const&context, Category category)const override;
    };
}