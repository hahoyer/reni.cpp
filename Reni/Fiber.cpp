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


Ref<CodeItem> const FiberHead::Fiber(Array<Ref<FiberItem>> const&items)const{
    return *Reni::Fiber::Create(ref, items);
}


virtual_p_implementation(FiberItem, Size, inSize){
    return Size(0);
}

virtual_p_implementation(FiberItem, Size, outSize){
    return Size(0);
}


String const FiberItem::ToCpp(CodeVisitor const& visitor)const{
    md(visitor);
    mb;
};


override_p_implementation(Fiber, Size, size){
    return items.Last->outSize;
};

Ref<CodeItem> const Fiber::Replace(ReplaceVisitor const&arg) const{
    md(arg);
    mb;
};

