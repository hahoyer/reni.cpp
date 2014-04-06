#include "Import.h"
#include "ExpressionSyntax.h"

#include "Feature.h"
#include "SyntaxContainer.h"

using namespace Reni;
static bool Trace = true;


ExpressionSyntax::ExpressionSyntax(TokenClass const& tokenClass, Ref<Syntax, true> const left, SourcePart const part, Ref<Syntax, true > const right)
    : baseType(part)
      , tokenClass(tokenClass)
      , left(left)
      , right(right)
{
    SetDumpString();
};

p_implementation(ExpressionSyntax, Array<String>, DumpData)
{
    return{
        nd(left),
        nd(tokenClass),
        nd(right)
    };
};

ResultData const ExpressionSyntax::GetResultData(Context const&context, Category category)const
{
    bool Trace = false;
    md(context,category);
    auto searchResult = context.Search(left, tokenClass);
    a_if_(searchResult.IsValid);
    d(searchResult);
    auto result = searchResult
        .feature
        ->FunctionResult(context, category, *this);
    a_is(category, <=, result.complete);
    return_d(result);
}

Ref<Syntax, true> const ExpressionSyntax::Replace(SyntaxArgVisitor const&visitor) const{
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

void ExpressionSyntax::AddTo(SyntaxContainer&main) const{
    main.statements += thisRef;
}