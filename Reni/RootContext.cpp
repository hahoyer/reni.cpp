#include "Import.h"
#include "RootContext.h"

#include "BitType.h"
#include "ContextFeature.h"
#include "ContextFeatureProvider.h"
#include "FeatureProvider.h"
#include "Result.h"
#include "SignFeatureProvider.h"
#include "Syntax.h"
#include "VoidType.h"
#include "../HWLib/RefCountContainer.instance.h"

using namespace Reni;
static bool Trace = true;


RootContext::RootContext()
: bitTypeInstance(new BitType())
, voidTypeInstance(new VoidType()){
    SetDumpString();
}

p_implementation(RootContext, WeakRef<BitType>, bitType){ return bitTypeInstance->ref; }
p_implementation(RootContext, WeakRef<VoidType>, voidType){ return voidTypeInstance->ref; }

RootContext::operator Ref<ContextFeatureProvider<SignToken>, true>() const{ return new SignFeatureProvider; }

override_p_implementation(RootContext, Array<String>, DumpData){
    return{};
};


