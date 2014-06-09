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
    public:
        DefineableToken const& tokenClass;
        Optional<Ref<Syntax>> const left;
        Optional<Ref<Syntax>> const right;
        ExpressionSyntax(DefineableToken const& tokenClass, Optional<Ref<Syntax>> const left, SourcePart const part, Optional<Ref<Syntax>> const right);
    private:
        p_function(String, SmartDump) override;
        p_function(int, priority) override;
        virtual ResultData const GetResultData(Context const&context, Category category)const override;
        virtual Optional<Ref<Syntax>> const Replace(SyntaxArgVisitor const&) const override;
        void AddTo(SyntaxContainer&) const override;
    };
}