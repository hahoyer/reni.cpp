#pragma once
#include "Syntax.h"
using namespace HWLib;

#include "Context.h"
#include "SearchResult.h"

namespace Reni
{
    class IfThenElseSyntax final: public Syntax
    {
        typedef Syntax baseType;
        typedef IfThenElseSyntax  thisType;
    public:
        Ref<Syntax> const condition;
        Ref<Syntax> const thenClause;
        Ref<Syntax> const elseClause;
        IfThenElseSyntax(Ref<Syntax> const condition, Ref<Syntax> const thenClause, SourcePart const& part, Ref<Syntax> const elseClause);
    private:
        HW_PR_DECL_GETTER(string, SmartDump) override;
        HW_PR_DECL_GETTER(int, priority) override{ return -10; }
        virtual ResultData GetResultData(const Context& context, Category const& category)const override;
        virtual Optional<Ref<Syntax>> Replace(SyntaxArgVisitor const&) const override;
    };

    class IfThenSyntax final : public Syntax
    {
        typedef Syntax baseType;
        typedef IfThenSyntax thisType;
    public:
        Ref<Syntax> const condition;
        Ref<Syntax> const thenClause;
        IfThenSyntax(Ref<Syntax> const left, SourcePart const part, Ref<Syntax> const right);

        Ref<Syntax> const AddElse(SourcePart const part, Ref<Syntax> const clause)const{
            return new IfThenElseSyntax(condition, thenClause, part, clause);
        }
    private:
        HW_PR_DECL_GETTER(string, SmartDump) override;
        HW_PR_DECL_GETTER(int, priority) override{ return -10; }
        virtual ResultData GetResultData(const Context& context, Category const& category)const override;
        virtual Optional<Ref<Syntax>> Replace(SyntaxArgVisitor const&) const override;
    };

}