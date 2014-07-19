#include "Import.h"
#include "AccessType.h"

#include "AccessData.h"
#include "ContainerContext.h"
#include "ReplaceVisitor.h"
#include "SyntaxContainer.h"

using namespace Reni;
static bool Trace = true;


AccessType::AccessType(AccessData const& data)
    : data(data.thisRef)
{
}

p_implementation(AccessType, Array<String>, DumpData) { return data->p_name(DumpData)(); }
p_implementation(AccessType, WeakRef<Global>, global){ return data->global; }
p_implementation(AccessType, WeakRef<Type>, value){return data->dataResultCache->type;}
p_implementation(AccessType, WeakPtr<NumberType>, asNumberType){ return value->As<NumberType>(); }

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

WeakPtr<AccessType> const AccessType::Convert(Type const& target)
{
    return dynamic_cast<AccessType*>(&target.thisRef);
}


ResultData const AccessType::AssignmentFeature::Result(Category category, Type const& target, Type const& arg) const
{
    auto typedTarget = Convert(target);
    auto rawResult = typedTarget->data->SetResultData(category);
    if(category <= Category::Type.replenished)
        return rawResult;

    ReplaceVisitor visitor;
    visitor.SetResults(External::Arg::Instance, *arg.ConvertTo(typedTarget->value->thisRef));
    return rawResult.Replace(visitor);
}