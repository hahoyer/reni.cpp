#include "Import.h"
#include "ExpressionSyntax.h"

using namespace Reni;
static bool Trace = true;


ExpressionSyntax::ExpressionSyntax(TokenClass const& tokenClass, OptRef<Syntax > const left, SourcePart const part, OptRef<Syntax > const right)
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
    auto result = searchResult.FunctionResult(context, category, *this);
    a_if_(category <= result.complete);
    return result;
}