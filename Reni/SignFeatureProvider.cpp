#include "Import.h"
#include "SignFeatureProvider.h"

using namespace Reni;
static bool Trace = true;


ResultData const SignFeatureProvider::Feature::FunctionResult(
    Context const&context,
    Category category,
    TokenClass const&tokenClass,
    Ref<Syntax, true> const& right
    )const {
    if(right.IsEmpty)
        return baseType::FunctionResult(context, category, tokenClass, right);
    return right->SignMacro(tokenClass)->GetResultData(context, category);
}

