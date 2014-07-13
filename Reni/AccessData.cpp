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
    , type(new AccessType(*this))
{
    SetDumpString();
}

p_implementation(AccessData, WeakRef<Global>, global){ return container.global; }

