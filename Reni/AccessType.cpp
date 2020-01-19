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

p_implementation(AccessType, Array<String>, DumpData) { return data->p_name(DumpData)(); }
p_implementation(AccessType, WeakRef<Global>, global){ return data->global; }
p_implementation(AccessType, WeakRef<Type>, value){return data->dataResultCache->type;}
p_implementation(AccessType, Optional<WeakRef<NumberType>>, asNumberType){ return value->As<NumberType>(); }

SearchResult<Feature> const AccessType::DeclarationsForType(DeclarationType const& target) const
{
    auto dircetResult = target.Declarations(*this);
    if(dircetResult.IsValid)
        return dircetResult;

    auto parentResult = value->DeclarationsForType(target);
    if(parentResult.IsValid)
        return parentResult.found + thisRef;

    return{};
}

Ref<ResultCache> const AccessType::DirectConvert() const
{
    return new FunctionResultCache
        ([&](Category category) -> ResultData const
    {
        return ResultData::GetSmartHllwSizeExts
            (
            category,
            l_(DirectConvertCode()),
            l_(WeakRef<Type>(thisRef))
            );
    }
    );
}

Ref<CodeItem> const AccessType::DirectConvertCode() const
{
    return CodeItem::Arg(*value->indirectType);
}

Optional<WeakRef<AccessType>> const AccessType::Convert(Type const& target)
{
    auto result = dynamic_cast<AccessType*>(&target.thisRef);
    if(result)
        return WeakRef<AccessType>(result);
    return{};
}


ResultData const AccessType::AssignmentFeature::Result(Category category, Type const& target, Type const& arg) const
{
    bool Trace = false;
    md(category, target, arg);
    auto typedTarget = Convert(target);
    auto rawResult = typedTarget.Value->data->SetResultData(category);
    if(category <= Category::Type.replenished)
        return_d(rawResult);

    b_if(Trace, nd(rawResult));

    ReplaceVisitor visitor;
    visitor.SetResults(External::Args::Instance, *arg.ConvertTo(typedTarget.Value->value->thisRef));
    auto result = rawResult.Replace(visitor);
    return_db(result);
}