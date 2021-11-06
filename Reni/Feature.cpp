#include "Import.h"
#include "Feature.h"

#include "AccessFeature.h"
#include "CodeItems.h"
#include "ContainerContext.h"
#include "DefineableToken.h"
#include "Syntax.h"
#include "Global.h"
#include "NumberType.h"
#include "ReplaceVisitor.h"

#include "../HWLib/RefCountContainer.instance.h"
#include "../HWLib/Ref.h"

static bool Trace = true;

using namespace HWLib;
using namespace Reni;


FoundFeature<Feature> Feature::Error(string const& title)
{
    HW_CONSOLE .Write(title);
    HW_BREAK_AND_THROW;
}

ResultData Feature::ConversionResult(const Category& category, Type const& target, Type const& destination) const
{
    return extended.Value->Result(category, target, destination);
};


AccessFeature::AccessFeature(Simple const& simple, Extended const& extended)
    : simple(simple.thisRef)
    , extended(extended.thisRef) 
{}

ResultData AccessFeature::FunctionResult(
  Context const& context,
  const Category& category,
  Optional<Ref<Syntax>> const& right
) const
{
    if(right.IsEmpty)
        return simple.Value->Result(context, category);

    ReplaceVisitor visitor;
    Optional<Ref<ResultFromSyntaxAndContext>> const argResult = right.Value->GetResultCache(context);
    visitor.SetResults(External::Args::Instance, *argResult.Value);
    auto rawResult = extended.Value->Result(context, category, *right.Value->GetType(context));
    auto result = rawResult.Replace(visitor);
    return(result);
}


ResultData EnableCutFeature::Result(Category const& category, Type const& target) const
{
    return target
        .enableCutType
        ->GetResultDataSmartClosure(category, l_(CodeItem::This(target)));
}


ResultData DumpPrintFeature::Result(Category const& category, Type const& target) const
{
    return target
        .global
        ->voidType
        .GetResultDataSmartClosure
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

ResultData FoundFeature<Feature>::AlignThis(ResultData const& start) const
{
    if(path.Count == 0)
        return start;

    if(path.Count == 1)
    {
        auto resultingThis = path[0];
        return start.Replace(External::This::Instance, *resultingThis->DirectConvert());
    }

    HW_D_METHOD(start);
    HW_BREAK_AND_THROW;
    return{};
}

ResultData FoundFeature<Feature>::FunctionResult(
  Context const& context,
  const Category& category,
  Optional<Ref<Syntax>> const& left,
  Optional<Ref<Syntax>> const& right
) const
{
    bool Trace = context.ObjectId == 3
        && left.IsValid && left.Value->ObjectId >- 9
        && category.hasCode
        && !context.isRecursion;
    HW_D_METHOD(context, category, left, right);

    auto rawResult = Result(context, category, type->thisRef, right);
    a_is(category, == , rawResult.complete);
    HW_D_LOG_VALUE(rawResult);
    b_if_(Trace);
    auto alignedResult = AlignThis(rawResult);
    HW_D_LOG_VALUE(alignedResult);
    b_if_(Trace);
    auto leftResult = left.Value->GetResultCache(context);
    leftResult->Trace = Trace;
    auto result = alignedResult
        .Replace
        (
            External::This::Instance,
                *leftResult
        );
    return_db(result);
};

ResultData FoundFeature<Feature>::Result(Context const& context, const Category& category, Type const& target,
                                         Optional<Ref<Syntax>> const& arg) const
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

FoundFeature<Feature> FoundFeature<Feature>::operator+(Type const& fromType) const
{
    return FoundFeature<Feature>(type->thisRef, feature, path + WeakRef<Type>(fromType.thisRef));
}

ResultData FoundFeature<Feature>::ConversionResult(const Category& category, Type const& target, Type const& destination) const
{
    return feature.ConversionResult(category, target, destination);
};

