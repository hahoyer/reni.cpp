#include "Import.h"
#include "Result.h"
#include "ExpressionSyntax.h"


static bool Trace = true;

using namespace HWLib;
using namespace Reni;


p_implementation(SearchResult, bool, IsValid){
    md_;
    b_;
    return false;
};

ResultData const SearchResult::FunctionResult(Context const&context, Category category, ExpressionSyntax const& expressionSyntax)const{
    md(context, category, expressionSyntax);
    b_;
    return{};
};