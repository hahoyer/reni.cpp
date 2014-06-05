#include "Import.h"
#include "Feature.h"

#include "ArgVisitor.h"
#include "CodeItems.h"
#include "ContextFeature.h"
#include "DefineableToken.h"
#include "ExpressionSyntax.h"
#include "Global.h"
#include "NumberType.h"
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
)const
{
    bool Trace = expressionSyntax.ObjectId == 3 && category.hasExternals;
    md(context, category, expressionSyntax.left, expressionSyntax.tokenClass, expressionSyntax.right);
    auto thisResult = expressionSyntax.left->GetResultCache(context);
    ArgVisitor visitor;
    visitor.Assign(&ArgVisitor::Tag::expressionThis, *thisResult);

    Ref<ResultFromSyntaxAndContext, true> argResult;
    if(!expressionSyntax.right.IsEmpty)
    {
        argResult = expressionSyntax.right->GetResultCache(context);
        visitor.Assign(&ArgVisitor::Tag::expressionArg, *argResult);
    }

    b_if_(Trace);
    auto rawResult = Result(category, *thisResult->type, argResult) & category;
    a_is(category, == , rawResult.complete);
    auto result = rawResult.Replace(visitor);
    return_d(result);
}

ResultData const Feature::Result(Category category, Type const& target, Ref<ResultFromSyntaxAndContext, true> argResult) const
{
    if(argResult.IsEmpty)
        return simple->Result(category, target);
    return extended->Result(category, target, *argResult->type);
};


ResultData const ContextFeature::FunctionResult(
    Context const&context,
    Category category,
    Ref<Syntax, true> const& right
)const
{
    if(right.IsEmpty)
        return simple->Result(context, category);
    return extended->Result(context, category, *right->Type(context));
}


ResultData const EnableCutFeature::Result(Category category, Type const&target)const
{
    return target
        .enableCutType
        ->GetResultData(category,l_(CodeItem::This(target, 0)));
}


ResultData const DumpPrintFeature::Result(Category category, Type const&target)const
{
    return target
        .global
        ->voidType
        .GetResultData(category, l_(CodeItem::DumpPrint(dynamic_cast<NumberType const&>(target))));
}