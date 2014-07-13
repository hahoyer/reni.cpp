#include "Import.h"
#include "AccessType.h"

#include "AccessData.h"
#include "ContainerContext.h"
#include "SyntaxContainer.h"

using namespace Reni;
static bool Trace = true;


AccessType::AccessType(AccessData const& data)
    : data(data.thisRef)
{
}

p_implementation(AccessType, Array<String>, DumpData) { return data->p_name(DumpData)(); }
p_implementation(AccessType, WeakRef<Global>, global){ return data->global; }

p_implementation(AccessType, WeakRef<Type>, value)
{
    md_;
    mb;
}


SearchResult<Feature> const AccessType::DeclarationsForType(DeclarationType const& target) const
{
    auto result = value->DeclarationsForType(target);
    if(result.IsValid)
        return result;
    return{};
}
