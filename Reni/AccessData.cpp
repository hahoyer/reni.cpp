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

p_implementation(AccessData, WeakRef<Global>, global){ return container.global; }


p_implementation(AccessData, Closure, XetClosure){ return Closure(container.asExternal); }
p_implementation(AccessData, bool, GetHollow){ return dataResultCache->hollow; }
p_implementation(AccessData, bool, SetHollow){ return true; }
p_implementation(AccessData, Size, SetSize){ return 0; }
p_implementation(AccessData, WeakRef<Type>, SetType){ return global->voidType.thisRef; }

p_implementation(AccessData, Size, GetSize)
{
    if(dataResultCache->hollow)
        return 0;
    return Size::Address;
}

p_implementation(AccessData, WeakRef<Type>, GetType)
{
    if(dataResultCache->hollow)
        return &dataResultCache->type->thisRef;
    return new AccessType(*this);
}

p_implementation(AccessData, Ref<CodeItem>, GetCode)
{
    return ThisCode;
}

p_implementation(AccessData, Ref<CodeItem>, SetCode)
{
    return CodeItem::Assign(dataResultCache->type->thisRef, ThisCode);
}

p_implementation(AccessData, Ref<CodeItem>, ThisCode)
{
    auto backOffset = Size(0);
    for(auto position = 0; position <= statementIndex; position++)
        backOffset -= container.AlignedPartSize(position);
    return CodeItem::Reference(container)->ReferencePlus(backOffset);
}

