#include "Import.h"
#include "Feature.h"
#include "ContextFeature.h"
#include "ExpressionSyntax.h"
#include "Result.h"
#include "../HWLib/Ref.h"

static bool Trace = true;

using namespace HWLib;
using namespace Reni;


ResultData const Feature::FunctionResult(
    Context const&context,
    Category category,
    ExpressionSyntax const& expressionSyntax
)const{
    md(context, category, expressionSyntax);
    b_;
    return{};
};


ResultData const ContextFeature::FunctionResult(
    Context const&context,
    Category category,
    ExpressionSyntax const& expressionSyntax
)const{
    a_if(expressionSyntax.left.IsEmpty, nd(expressionSyntax.left));    
    return FunctionResult(context, category, expressionSyntax.tokenClass,expressionSyntax.right);
}

ResultData const ContextFeature::FunctionResult(
    Context const&context,
    Category category,
    TokenClass const&tokenClass,
    Ref<Syntax, true> const& right
)const{
    md(context, category, tokenClass, right);
    b_;
    return{};
}
