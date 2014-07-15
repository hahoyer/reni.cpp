#include "Import.h"
#include "AccessData.h"

#include "AccessResultCache.h"
#include "AccessType.h"
#include "CodeItem.h"
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
        l_(GetHllw),
        l_(GetSize),
        l_(GetCode),
        l_(GetType),
        l_(XetExts)
        );
}

ResultData const AccessData::SetResultData(Category category) const
{
    return ResultData::Get
        (
        category,
        l_(SetHllw),
        l_(SetSize),
        l_(SetCode),
        l_(SetType),
        l_(XetExts)
        );
}

p_implementation(AccessData, WeakRef<Global>, global){ return container.global; }


p_implementation(AccessData, Externals, XetExts){ return Externals(container.asExternal); }
p_implementation(AccessData, bool, GetHllw){ return dataResultCache->hllw; }
p_implementation(AccessData, bool, SetHllw){ return true; }
p_implementation(AccessData, Size, SetSize){ return 0; }
p_implementation(AccessData, WeakRef<Type>, SetType){ return global->voidType.thisRef; }

p_implementation(AccessData, Size, GetSize)
{
    if(dataResultCache->hllw)
        return 0;
    return Size::Address;
}

p_implementation(AccessData, WeakRef<Type>, GetType)
{
    if(dataResultCache->hllw)
        return &dataResultCache->type->thisRef;
    return new AccessType(*this);
}

p_implementation(AccessData, Ref<CodeItem>, GetCode)
{
    auto xx = ThisCode;
    md(xx);
    mb;
}

p_implementation(AccessData, Ref<CodeItem>, SetCode)
{
    auto xx = ThisCode;
    md(xx);
    mb;
}

p_implementation(AccessData, Ref<CodeItem>, ThisCode)
{
    auto backOffset = Size(0);
    for(auto position = 0; position <= statementIndex; position++)
        backOffset -= container.PartSize(position);
    return CodeItem::Reference(container)->ReferencePlus(backOffset);
}

