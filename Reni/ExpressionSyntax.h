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
        HW_PR_DECL_GETTER(string, SmartDump) override;
        HW_PR_DECL_GETTER(int, priority) override;
        virtual ResultData GetResultData(const Context& context, Category const& category)const override;
        virtual Optional<Ref<Syntax>> Replace(SyntaxArgVisitor const&) const override;
        void AddTo(SyntaxContainer&) const override;
    };
}