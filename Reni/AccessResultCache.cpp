#include "Import.h"
#include "AccessResultCache.h"

#include "AccessData.h"
#include "ContainerContext.h"
#include "SyntaxContainer.h"
#include "Type.h"

using namespace Reni;
static bool Trace = true;


AccessResultCache::AccessResultCache(AccessData const&data)
    : data(&data.thisRef)
{
}

p_implementation(AccessResultCache, Array<String>, DumpData){ return data->p_name(DumpData)(); }

ResultData const AccessResultCache::GetResultData(Category category) const{return data->GetResultData(category);}