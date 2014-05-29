#include "Import.h"
#include "ExpressionSyntax.h"

#include "DefineableToken.h"
#include "Feature.h"
#include "SyntaxContainer.h"

using namespace Reni;
static bool Trace = true;


ExpressionSyntax::ExpressionSyntax(DefineableToken const& tokenClass, Ref<Syntax, true> const left, SourcePart const part, Ref<Syntax, true > const right)
    : baseType(part)
      , tokenClass(tokenClass)
      , left(left)
      , right(right)
{
    SetDumpString();
};

p_implementation(ExpressionSyntax, String, SmartDump)
{
    return
        (left.IsEmpty? "" : (left->SmartDumpFrame(priority)+ " "))
        + tokenClass.name
        + (right.IsEmpty?"":(" "+ right->SmartDumpFrame(priority)));
};

p_implementation(ExpressionSyntax, int, priority) { return tokenClass.priority; };

ResultData const ExpressionSyntax::GetResultData(Context const&context, Category category)const
{
    if(left.IsEmpty)
    {
        auto result = context
            .Search(tokenClass)
            .feature
            .FunctionResult(context, category, right);
        a_is(category, <= , result.complete);
        return(result);
    }

    auto result = left
        ->Type(context)
        ->thisRef
        .Search(tokenClass)
        .feature
        .FunctionResult(context, category, *this);
    a_is(category, <=, result.complete);
    return(result);
}

Ref<Syntax, true> const ExpressionSyntax::Replace(SyntaxArgVisitor const&visitor) const
{
    Ref<Syntax, true> newLeft;
    if(!left.IsEmpty)
        newLeft = left->Replace(visitor);
    Ref<Syntax, true> newRight;
    if(!right.IsEmpty)
        newRight = right->Replace(visitor);

    if(newLeft.IsEmpty && newRight.IsEmpty)
        return{};

    return new ExpressionSyntax(tokenClass, newLeft || left, part, newRight || right);
}

void ExpressionSyntax::AddTo(SyntaxContainer&main) const
{
    main.statements += thisRef;
}
