#include "Import.h"
#include "Fiber.h"

#include "FeatureProvider.h"
#include "Result.h"
#include "Size.h"
#include "../HWLib/RefCountContainer.instance.h"
#include "ReplaceVisitor.h"

using namespace Reni;
using namespace HWLib;

static bool Trace = true;


virtual_p_implementation(FiberItem, Size, inSize){
    return Size(0);
}

virtual_p_implementation(FiberItem, Size, outSize){
    return Size(0);
}


Ref<FiberItem> const FiberItem::Replace(ReplaceVisitor const&visitor) const {
    md(visitor);
    mb;
};


String const FiberItem::ToCpp(CodeVisitor const& visitor)const{
    md(visitor);
    mb;
};


override_p_implementation(Fiber, Size, size){
    return items.Last->outSize;
};

Ref<CodeItem> const Fiber::Replace(ReplaceVisitor const&visitor) const {
    Ref<CodeItem> newHead = head->Replace(visitor);
    Array<Ref<FiberItem>> newItems = items
        .Select<Ref<FiberItem>>([&](Ref<FiberItem> item) {return item->Replace(visitor); })
        ->ToArray;
    return *ReCreate(newHead, newItems);
};

Ref<Fiber> Fiber::ReCreate(Ref<CodeItem> const&head, Array<Ref<FiberItem>> const& items)const{
    md(head, items);
    mb;
}
