#include "Import.h"
#include "Feature.h"

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
