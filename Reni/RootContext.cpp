#include "Import.h"
#include "RootContext.h"

#include "BitType.h"
#include "ContextFeature.h"
#include "ContextFeatureProvider.h"
#include "FeatureProvider.h"
#include "Result.h"
#include "MinusFeatureProvider.h"
#include "Syntax.h"
#include "VoidType.h"
#include "../HWLib/RefCountContainer.instance.h"

using namespace Reni;
static bool Trace = true;

class RootContext::internal final{
public:
    internal()
    : minus(new MinusFeatureProvider){};
    BitType const bitType;
    VoidType const voidType;
    Ref<MinusFeatureProvider> const minus;
};


RootContext::RootContext()
: _internal(new internal){}

p_implementation(RootContext, WeakRef<BitType>, bitType){ return _internal->bitType.ref; }
p_implementation(RootContext, WeakRef<VoidType>, voidType){ return _internal->voidType.ref; }

RootContext::operator Ref<ContextFeatureProvider<MinusToken>, true>() const{ return _internal->minus->ref; }

p_implementation(RootContext, Array<String>, DumpData){
    return{};
};


