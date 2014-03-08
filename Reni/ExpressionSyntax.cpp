#include "Import.h"
#include "ExpressionSyntax.h"

#include "Feature.h"

using namespace Reni;
static bool Trace = true;


ExpressionSyntax::ExpressionSyntax(TokenClass const& tokenClass, Ptr<Syntax > const left, SourcePart const part, Ptr<Syntax > const right)
    : baseType(part)
      , tokenClass(tokenClass)
      , left(left)
      , right(right)
{
    SetDumpString();
};

override_p_implementation(ExpressionSyntax, Array<String>, DumpData)
{
    return{
        nd(left),
        nd(tokenClass),
        nd(right)
    };
};

ResultData const ExpressionSyntax::GetResultData(Context const&context, Category category)const
{
    auto searchResult = context.Search(left, tokenClass);
    a_if_(searchResult.IsValid);
    auto result = searchResult
        .feature
        ->FunctionResult(context, category, *this);
    a_if_(category <= result.complete);
    return result;
}