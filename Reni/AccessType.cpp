#include "Import.h"
#include "AccessType.h"

#include "AccessData.h"
#include "CodeItems.h"
#include "ContainerContext.h"
#include "ReplaceVisitor.h"
#include "SyntaxContainer.h"
#include "FunctionResultCache.h"

using namespace Reni;
static bool Trace = true;


AccessType::AccessType(AccessData const& data)
  : data(data.thisRef)
{
  SetDumpString();
}

HW_PR_IMPL_GETTER(AccessType, Array<string>, DumpData) { return data->HW_PR_GETTER_NAME(DumpData)(); }
HW_PR_IMPL_GETTER(AccessType, WeakRef<Global>, global) { return data->global; }
HW_PR_IMPL_GETTER(AccessType, WeakRef<Type>, value) { return data->dataResultCache->type; }
HW_PR_IMPL_GETTER(AccessType, Optional<WeakRef<NumberType>>, asNumberType) { return value->As<NumberType>(); }

SearchResult<Feature> AccessType::DeclarationsForType(DeclarationType const& target) const
{
  auto dircetResult = target.Declarations(*this);
  if(dircetResult.IsValid)
    return dircetResult;

  auto parentResult = value->DeclarationsForType(target);
  if(parentResult.IsValid)
    return parentResult.found + thisRef;

  return {};
}

Ref<ResultCache> AccessType::DirectConvert() const
{
  return new FunctionResultCache([&](Category category) -> ResultData const
    {
      return ResultData::GetSmartHollowSizeClosure
      (
        category,
        l_(DirectConvertCode()),
        l_(WeakRef<Type>(thisRef))
      );
    }
  );
}

Ref<CodeItem> AccessType::DirectConvertCode() const
{
  return CodeItem::Arg(*value->indirectType);
}

Optional<WeakRef<AccessType>> AccessType::Convert(Type const& target)
{
  if(const auto result = dynamic_cast<AccessType*>(&target.thisRef))
    return WeakRef<AccessType>(result);
  return {};
}


ResultData AccessType::AssignmentFeature::Result(Category const& category, Type const& target, Type const& arg) const
{
  bool Trace = false;
  HW_D_METHOD(category, target, arg);
  auto typedTarget = Convert(target);
  auto rawResult = typedTarget.Value->data->SetResultData(category);
  if(category <= Category::Type.replenished)
    return_d(rawResult);

  HW_BREAK_IF(Trace, HW_D_VALUE(rawResult));

  ReplaceVisitor visitor;
  visitor.SetResults(External::Args::Instance, *arg.ConvertTo(typedTarget.Value->value->thisRef));
  auto result = rawResult.Replace(visitor);
  return_db(result);
}
