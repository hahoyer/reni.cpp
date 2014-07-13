#include "Import.h"
#include "AccessData.h"

#include "AccessResultCache.h"
#include "AccessType.h"
#include "ContainerContext.h"
#include "ContextIncludes.h"
#include "FunctionSyntax.h"
#include "Result.h"
#include "SyntaxContainer.h"

using namespace Reni;
static bool Trace = true;


AccessData::AccessData(ContainerContext const& container, int statementIndex)
    : statementIndex(statementIndex)
    , container(container)
    , dataResultCache
    (
        new ResultFromSyntaxAndContext
        (
            container.containerData->statements[statementIndex]->thisRef,
            container
        )
    )
    , resultCache(new AccessResultCache(*this))
    , feature
    (
        new AccessFeature
        (
            *new SimpleFeature(*this),
            *new ExtendedFeature(container, statementIndex)
        )

    )
{
    SetDumpString();
}

ResultData const AccessData::GetResultData(Category category) const
{
    return ResultData::Get
        (
        category,
        l_(GetHllw()),
        l_(GetSize()),
        l_(GetCode()),
        l_(GetType()),
        l_(GetExts())
        );
}

p_implementation(AccessData, WeakRef<Global>, global){ return container.global; }


bool const AccessData::GetHllw() const{return dataResultCache->hllw;}
Externals const AccessData::GetExts() const{ return{}; }

Size const AccessData::GetSize() const
{
    if(dataResultCache->hllw)
        return 0;
    return Size::Address;
}

Ref<CodeItem> const AccessData::GetCode() const
{
    md_;
    mb;
}

WeakRef<Type> const AccessData::GetType()const
{
    if(dataResultCache->hllw)
        return &dataResultCache->type->thisRef;
    return new AccessType(*this);
}


