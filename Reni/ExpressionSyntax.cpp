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

HW_PR_IMPL_GETTER(ExpressionSyntax, string, SmartDump)
{
    return
        (left.IsEmpty? "" : (left.Value->SmartDumpFrame(priority)+ " "))
        + tokenClass.name
        + (right.IsEmpty ? "" : (" " + right.Value->SmartDumpFrame(priority)));
};

HW_PR_IMPL_GETTER(ExpressionSyntax, int, priority) { return tokenClass.priority; };

ResultData ExpressionSyntax::GetResultData(const Context& context, Category const& category)const
{
    if(left.IsEmpty)
    {
        auto result = context
            .DeclarationsForType(tokenClass)
            .found
            .FunctionResult(context, category, right);
        HW_ASSERT_IS(category, <= , result.complete);
        return(result);
    }

    auto result = left
        .Value
        ->GetType(context)
        ->thisRef
        .DeclarationsForType(tokenClass)
        .found
        .FunctionResult(context, category, left, right);
    HW_ASSERT_IS(category, <=, result.complete);
    return(result);
}

Optional<Ref<Syntax>> ExpressionSyntax::Replace(SyntaxArgVisitor const&visitor) const
{
    Optional<Ref<Syntax>> newLeft;
    if(!left.IsEmpty)
        newLeft = left.Value->Replace(visitor);
    Optional<Ref<Syntax>> newRight;
    if(!right.IsEmpty)
        newRight = right.Value->Replace(visitor);

    if(newLeft.IsEmpty && newRight.IsEmpty)
        return{};

    return Ref<Syntax>(new ExpressionSyntax(tokenClass, newLeft || left, part, newRight || right));
}

void ExpressionSyntax::AddTo(SyntaxContainer&main) const
{
    main.statements += thisRef;
}
