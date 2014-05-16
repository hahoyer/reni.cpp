#include "Import.h"
#include "Feature.h"
#include "ContextFeature.h"
#include "DefineableToken.h"
#include "ExpressionSyntax.h"
#include "Result.h"
#include "../HWLib/RefCountContainer.instance.h"
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
    return FunctionResult(context, category, expressionSyntax.right);
}

ResultData const ContextFeature::FunctionResult(
    Context const&context,
    Category category,
    Ref<Syntax, true> const& right
)const{
    md(context, category, right);
    b_;
    return{};
}


ResultData const InfixFunctionFeature::FunctionResult(
    Context const&context,
    Category category,
    ExpressionSyntax const& expressionSyntax
    )const 
{
    bool Trace = expressionSyntax.ObjectId == 13;
    md(context, category, expressionSyntax);
    a_throw_(!expressionSyntax.right.IsEmpty);
    auto thisResult = expressionSyntax.left->GetResultCache(context);
    auto argResult = expressionSyntax.right->GetResultCache(context);
    d(thisResult);
    b_if_(Trace);
    auto result = Result(category, *thisResult->type, *argResult->type);
    d(result);
    ArgVisitor visitor;
    visitor.Assign(&ArgVisitor::Tag::expressionThis, *thisResult);
    visitor.Assign(&ArgVisitor::Tag::expressionArg, *argResult);
    return_d(result.Replace(visitor));
};

