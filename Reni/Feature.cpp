#include "Import.h"
#include "Feature.h"

#include "AccessFeature.h"
#include "Address.h"
#include "CodeItems.h"
#include "ContainerContext.h"
#include "DefineableToken.h"
#include "Syntax.h"
#include "Global.h"
#include "NumberType.h"
#include "ReplaceVisitor.h"
#include "SyntaxContainer.h"

#include "../HWLib/RefCountContainer.instance.h"
#include "../HWLib/Ref.h"

static bool Trace = true;

using namespace HWLib;
using namespace Reni;


Feature const Feature::Error(String const&title)
{
    _console_ .Write(title);
    mb;
}

ResultData const Feature::FunctionResult(
    Context const&context,
    Category category,
    Optional<Ref<Syntax>> const&left,
    Optional<Ref<Syntax>> const&right
    )const
{
    bool Trace = context.ObjectId == -6
        && left.IsValid && left.Value->ObjectId == 8 
        && category.hasType
        && !context.isRecursion;
    md(context, category, left, right);
    auto thisResult = left.Value->GetResultCache(context);
    ReplaceVisitor visitor;
    visitor.Trace = Trace;
    visitor.SetResults(External::This::Instance, *thisResult);

    Optional<Ref<ResultFromSyntaxAndContext>> argResult;
    if(!right.IsEmpty)
    {
        argResult = right.Value->GetResultCache(context);
        visitor.SetResults(External::Arg::Instance, *argResult.Value);
    }

    b_if_(Trace);
    auto rawResult = Result(category, *thisResult->type, argResult);
    b_if(Trace,nd(rawResult));
    a_is(category, == , rawResult.complete);
    auto result = rawResult.Replace(visitor);
    return_d(result);
}

ResultData const Feature::Result(Category category, Type const& target, Optional<Ref<ResultFromSyntaxAndContext>> argResult) const
{
    if(argResult.IsEmpty)
        return simple.Value->Result(category, target);
    return extended.Value->Result(category, target, *argResult.Value->type);
}

ResultData const Feature::ConversionResult(Category category, Type const& target, Type const& destination) const
{
    return extended.Value->Result(category, target, destination);
};


AccessFeature::AccessFeature(Simple const& simple, Extended const& extended)
    : simple(simple.thisRef)
    , extended(extended.thisRef) 
{}

ResultData const AccessFeature::FunctionResult(
    Context const&context,
    Category category,
    Optional<Ref<Syntax>> const& right
    )const
{
    if(right.IsEmpty)
        return simple.Value->Result(context, category);

    ReplaceVisitor visitor;
    Optional<Ref<ResultFromSyntaxAndContext>> const argResult = right.Value->GetResultCache(context);
    visitor.SetResults(External::Arg::Instance, *argResult.Value);
    auto rawResult = extended.Value->Result(context, category, *right.Value->Type(context));
    auto result = rawResult.Replace(visitor);
    return(result);
}


ResultData const EnableCutFeature::Result(Category category, Type const&target)const
{
    return target
        .enableCutType
        ->GetResultDataSmartExts(category, l_(CodeItem::This(target)));
}


ResultData const DumpPrintFeature::Result(Category category, Type const& target)const
{
    return target
        .global
        ->voidType
        .GetResultDataSmartExts
        (
            category,
            l_(CodeItem::DumpPrint(dynamic_cast<NumberType const&>(target)))
        );
}
