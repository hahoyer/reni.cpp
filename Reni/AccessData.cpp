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


AccessData::AccessData(ContainerContext const& container, size_t statementIndex)
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
        l_(GetHollow),
        l_(GetSize),
        l_(GetCode),
        l_(GetType),
        l_(XetClosure)
        );
}

ResultData const AccessData::SetResultData(Category category) const
{
    return ResultData::Get
        (
        category,
        l_(SetHollow),
        l_(SetSize),
        l_(SetCode),
        l_(SetType),
        l_(XetClosure)
        );
}

HW_PR_IMPL_GETTER(AccessData, WeakRef<Global>, global){ return container.global; }


HW_PR_IMPL_GETTER(AccessData, Closure, XetClosure){ return Closure(container.asExternal); }
HW_PR_IMPL_GETTER(AccessData, bool, GetHollow){ return dataResultCache->hollow; }
HW_PR_IMPL_GETTER(AccessData, bool, SetHollow){ return true; }
HW_PR_IMPL_GETTER(AccessData, Size, SetSize){ return 0; }
HW_PR_IMPL_GETTER(AccessData, WeakRef<Type>, SetType){ return global->voidType.thisRef; }

HW_PR_IMPL_GETTER(AccessData, Size, GetSize)
{
    if(dataResultCache->hollow)
        return 0;
    return Size::Address;
}

HW_PR_IMPL_GETTER(AccessData, WeakRef<Type>, GetType)
{
    if(dataResultCache->hollow)
        return &dataResultCache->type->thisRef;
    return new AccessType(*this);
}

HW_PR_IMPL_GETTER(AccessData, Ref<CodeItem>, GetCode)
{
    return ThisCode;
}

HW_PR_IMPL_GETTER(AccessData, Ref<CodeItem>, SetCode)
{
    return CodeItem::Assign(dataResultCache->type->thisRef, ThisCode);
}

HW_PR_IMPL_GETTER(AccessData, Ref<CodeItem>, ThisCode)
{
    auto backOffset = Size(0);
    for(auto position = 0; position <= statementIndex; position++)
        backOffset -= container.AlignedPartSize(position);
    return CodeItem::Reference(container)->ReferencePlus(backOffset);
}

