#include "Import.h"
#include "AccessResultCache.h"

#include "ContainerContext.h"
#include "SyntaxContainer.h"
#include "Type.h"

using namespace Reni;
static bool Trace = true;


AccessResultCache::AccessResultCache(ContainerContext const& container, int statementIndex)
    : container(container)
    , statementIndex(statementIndex)
    , dataResultCache
    (
        new ResultFromSyntaxAndContext
        (
            container.containerData->statements[statementIndex]->thisRef,
            container
        )
    )
{
    SetDumpString();
}

ResultData const AccessResultCache::GetResultData(Category category) const
{
    if (category == Category::Type)
    {
        if (dataResultCache->hllw)
            return dataResultCache->type->thisRef;
    }


    md(category);
    mb;
    return{};
}