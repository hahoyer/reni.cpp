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
    bool Trace = expressionSyntax.ObjectId == -3 && category.hasExts;
    md(context, category, expressionSyntax.left, expressionSyntax.tokenClass, expressionSyntax.right);
    auto thisResult = expressionSyntax.left.Value->GetResultCache(context);
    ArgVisitor visitor;
    visitor.Trace = Trace;
    visitor.Assign(&ArgVisitor::Tag::expressionThis, *thisResult);

    Optional<Ref<ResultFromSyntaxAndContext>> argResult;
    if(!expressionSyntax.right.IsEmpty)
    {
        argResult = expressionSyntax.right.Value->GetResultCache(context);
        visitor.Assign(&ArgVisitor::Tag::expressionArg, *argResult.Value);
    }

    b_if_(Trace);
    auto rawResult = Result(category, *thisResult->type, argResult);
    d(rawResult);
    a_is(category, == , rawResult.complete);
    auto result = rawResult.Replace(visitor);
    return_d(result);
}

ResultData const Feature::Result(Category category, Type const& target, Optional<Ref<ResultFromSyntaxAndContext>> argResult) const
{
    if(argResult.IsEmpty)
        return simple.Value->Result(category, target);
    return extended.Value->Result(category, target, *argResult.Value->type);
};


ResultData const ContextFeature::FunctionResult(
    Context const&context,
    Category category,
    Optional<Ref<Syntax>> const& right
)const
{
    if(right.IsEmpty)
        return simple.Value->Result(context, category);
    return extended.Value->Result(context, category, *right.Value->Type(context));
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