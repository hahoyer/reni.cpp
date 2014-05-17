#include "Import.h"
#include "ConditionalSyntax.h"

#include "Feature.h"

using namespace Reni;
static bool Trace = true;


IfThenSyntax::IfThenSyntax(Ref<Syntax> const condition, SourcePart const part, Ref<Syntax> const thenClause)
: baseType(part)
, condition(condition)
, thenClause(thenClause)
{
    SetDumpString();
};

p_implementation(IfThenSyntax, String, SmartDump)
{
    return
        condition->SmartDumpFrame(priority)
        + " then "
        + thenClause->SmartDumpFrame(priority);
};

ResultData const IfThenSyntax::GetResultData(Context const&context, Category category)const
{
    md(context, category);
    b_;
    return{};
}

Ref<Syntax, true> const IfThenSyntax::Replace(SyntaxArgVisitor const&visitor) const{
    auto newLeft = condition->Replace(visitor);
    auto newRight = thenClause->Replace(visitor);
    if(newLeft.IsEmpty && newRight.IsEmpty)
        return{};
    return new IfThenSyntax(newLeft || condition, part, newRight || thenClause);
}


IfThenElseSyntax::IfThenElseSyntax(Ref<Syntax> const condition, Ref<Syntax> const thenClause, SourcePart const part, Ref<Syntax> const elseClause)
: baseType(part)
, condition(condition)
, thenClause(thenClause)
, elseClause(elseClause)
{
    SetDumpString();
};

p_implementation(IfThenElseSyntax, String, SmartDump)
{
    return
        condition->SmartDumpFrame(priority)
        + " then "
        + thenClause->SmartDumpFrame(priority)
        + " else "
        + elseClause->SmartDumpFrame(priority);
};

ResultData const IfThenElseSyntax::GetResultData(Context const&context, Category category)const
{
    if(category == Category::None)
    {
        auto thenType = thenClause->Type(context);
        auto elseType = elseClause->Type(context);
        md(thenType, elseType);
        b_;
    }

    if(category == Category::Type)
    {
        auto thenType = thenClause->Type(context);
        auto elseType = elseClause->Type(context);
        md(thenType, elseType);
        b_;
    }

    md(context, category);
    b_;
    return{};
}

Ref<Syntax, true> const IfThenElseSyntax::Replace(SyntaxArgVisitor const&visitor) const
{
    auto newLeft = condition->Replace(visitor);
    auto newRight = thenClause->Replace(visitor);
    auto newElse= elseClause->Replace(visitor);
    if(newLeft.IsEmpty && newRight.IsEmpty&& newElse.IsEmpty)
        return{};
    return new IfThenElseSyntax(newLeft || condition, newRight || thenClause, part, newElse || elseClause);
}
