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
        IfThenElseSyntax(Ref<Syntax> const condition, Ref<Syntax> const thenClause, SourcePart const part, Ref<Syntax> const elseClause);
    private:
        p_function(Array<String>, DumpData) override;
        virtual ResultData const GetResultData(Context const&context, Category category)const override;
        virtual Ref<Syntax, true> const Replace(SyntaxArgVisitor const&) const override;
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
        p_function(Array<String>, DumpData) override;
        virtual ResultData const GetResultData(Context const&context, Category category)const override;
        virtual Ref<Syntax, true> const Replace(SyntaxArgVisitor const&) const override;
    };

}