#include "Import.h"
#include "ExpressionSyntax.h"

#include "AccessFeature.h"
#include "ContainerContext.h"
#include "DefineableToken.h"
#include "Feature.h"
#include "SearchResult.h"
#include "SyntaxContainer.h"
#include "Type.h"

using namespace Reni;
static bool Trace = true;


ExpressionSyntax::ExpressionSyntax(DefineableToken const& tokenClass, Optional<Ref<Syntax>> const left, SourcePart const part, Optional<Ref<Syntax>> const right)
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
        (left.IsEmpty? "" : (left.Value->SmartDumpFrame(priority)+ " "))
        + tokenClass.name
        + (right.IsEmpty ? "" : (" " + right.Value->SmartDumpFrame(priority)));
};

p_implementation(ExpressionSyntax, int, priority) { return tokenClass.priority; };

ResultData const ExpressionSyntax::GetResultData(Context const&context, Category category)const
{
    if(left.IsEmpty)
    {
        auto result = context
            .DeclarationsForType(tokenClass)
            .found
            .FunctionResult(context, category, right);
        a_is(category, <= , result.complete);
        return(result);
    }

    auto result = left
        .Value
        ->Type(context)
        ->thisRef
        .DeclarationsForType(tokenClass)
        .found
        .FunctionResult(context, category, left, right);
    a_is(category, <=, result.complete);
    return(result);
}

Optional<Ref<Syntax>> const ExpressionSyntax::Replace(SyntaxArgVisitor const&visitor) const
{
    Optional<Ref<Syntax>> newLeft;
    if(!left.IsEmpty)
        newLeft = left.Value->Replace(visitor);
    Optional<Ref<Syntax>> newRight;
    if(!right.IsEmpty)
        newRight = right.Value->Replace(visitor);

    if(newLeft.IsEmpty && newRight.IsEmpty)
        return{};

    return new ExpressionSyntax(tokenClass, newLeft || left, part, newRight || right);
}

void ExpressionSyntax::AddTo(SyntaxContainer&main) const
{
    main.statements += thisRef;
}
