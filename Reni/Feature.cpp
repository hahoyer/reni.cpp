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
#include "FunctionResultCache.h"

static bool Trace = true;

using namespace HWLib;
using namespace Reni;


FoundFeature<Feature> const Feature::Error(String const&title)
{
    c_ .Write(title);
    mb;
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

ResultData const FoundFeature<Feature>::AlignThis(ResultData const& start) const
{
    if(path.Count == 0)
        return start;

    if(path.Count == 1)
    {
        auto resultingThis = path[0];
        return start.Replace(External::This::Instance, *resultingThis->DirectConvert());
    }

    md(start);
    mb;
    return{};
}

ResultData const FoundFeature<Feature>::FunctionResult(
    Context const& context,
    Category category,
    Optional<Ref<Syntax>> const& left,
    Optional<Ref<Syntax>> const& right
    )const
{
    bool Trace = context.ObjectId == 3
        && left.IsValid && left.Value->ObjectId >- 9
        && category.hasCode
        && !context.isRecursion;
    md(context, category, left, right);

    auto rawResult = Result(context, category, type->thisRef, right);
    a_is(category, == , rawResult.complete);
    d(rawResult);
    b_if_(Trace);
    auto alignedResult = AlignThis(rawResult);
    d(alignedResult);
    b_if_(Trace);
    auto result = alignedResult
        .Replace
        (
            External::This::Instance,
                *left.Value->GetResultCache(context)
        );
    return_db(result);
};

ResultData const FoundFeature<Feature>::Result(Context const& context, Category category, Type const& target, Optional<Ref<Syntax>> const& arg) const
{
    if(arg.IsEmpty)
        return feature.simple.Value->Result(category, target);

    auto argResult = arg.Value->GetResultCache(context);
    return feature
        .extended
        .Value
        ->Result(category, target, *argResult->type)
        .Replace(ReplaceVisitor(External::Args::Instance, *argResult))
        ;
}

FoundFeature<Feature> const FoundFeature<Feature>::operator+(Type const& fromType)const
{
    return FoundFeature<Feature>(type->thisRef, feature, path + WeakRef<Type>(fromType.thisRef));
}

ResultData const FoundFeature<Feature>::ConversionResult(Category category, Type const& target, Type const& destination) const
{
    return feature.ConversionResult(category, target, destination);
};

