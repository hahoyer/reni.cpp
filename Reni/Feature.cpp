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


FoundFeature<Feature> const Feature::Error(String const&title)
{
    _console_ .Write(title);
    mb;
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
    visitor.SetResults(External::Args::Instance, *argResult.Value);
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


FoundFeature<Feature>::FoundFeature(Type const& type, Feature const& feature, Array<WeakRef<Type>> const& path)
    : type(type.thisRef)
    , feature(feature)
    , path(path)
{
    SetDumpString();
}

FoundFeature<Feature>::FoundFeature(Type const& type, Feature const& feature)
    : thisType(type.thisRef, feature, {})
{
}

FoundFeature<Feature>::FoundFeature(FoundFeature const&other)
    : thisType(other.type->thisRef, other.feature, other.path)
{
}

ResultData const FoundFeature<Feature>::FunctionResult(
    Context const& context,
    Category category,
    Optional<Ref<Syntax>> const& left,
    Optional<Ref<Syntax>> const& right
    )const
{
    bool Trace = context.ObjectId == 6
        && left.IsValid && left.Value->ObjectId == 9
        && category.hasCode
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
        visitor.SetResults(External::Args::Instance, *argResult.Value);
    }

    auto rawResult = feature.Result(category, type->thisRef, argResult);
    a_is(category, == , rawResult.complete);
    d(visitor);
    d(rawResult);
    b_if_(Trace);
    auto result = rawResult.Replace(visitor);
    return_db(result);
};

FoundFeature<Feature> const FoundFeature<Feature>::operator+(Type const& fromType)const
{
    return FoundFeature<Feature>(type->thisRef, feature, path + WeakRef<Type>(fromType.thisRef));
}

ResultData const FoundFeature<Feature>::ConversionResult(Category category, Type const& target, Type const& destination) const
{
    return feature.ConversionResult(category, target, destination);
};

