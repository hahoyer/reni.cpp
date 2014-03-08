#include "Import.h"
#include "RootContext.h"

#include "BitType.h"
#include "FeatureProvider.h"
#include "VoidType.h"

using namespace Reni;
static bool Trace = true;


RootContext::RootContext()
: bitTypeInstance(new BitType())
, voidTypeInstance(new VoidType())
{}

p_implementation(RootContext, WeakRef<BitType>, bitType){ return bitTypeInstance->ref; }
p_implementation(RootContext, WeakRef<VoidType>, voidType){ return voidTypeInstance->ref; }

override_p_implementation(RootContext, Array<String>, DumpData){
    return{};
};

#include "../HWLib/RefCountContainer.instance.h"
