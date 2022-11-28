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

HW_PR_IMPL_GETTER(AccessResultCache, Array<string>,DumpData){ return data->HW_PR_GETTER_NAME(DumpData)(); }

ResultData AccessResultCache::GetResultData(Category const&category) const{return data->GetResultData(category);}