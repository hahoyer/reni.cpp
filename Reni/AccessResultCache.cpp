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

Array<String> const AccessResultCache::get_DumpData() const{ return data->p_name(DumpData)(); }

ResultData const AccessResultCache::GetResultData(Category category) const
{
    if (category == Category::Type)
    {
        if (data->dataResultCache->hllw)
            return data->dataResultCache->type->thisRef;
        return data->container.AccessType(data->statementIndex)->thisRef;
    }

    md(category);
    mb;
    return{};
}