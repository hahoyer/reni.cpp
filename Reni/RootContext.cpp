#include "Import.h"
#include "RootContext.h"

#include "BitType.h"
#include "ContextFeature.h"
#include "ContextFeatureProvider.h"
#include "FeatureProvider.h"
#include "VoidType.h"
#include "../HWLib/RefCountContainer.instance.h"

using namespace Reni;
static bool Trace = true;


class SignFeatureProvider final : public ContextFeatureProvider<SignToken>{
    typedef FeatureProvider<SignToken> baseType;
    typedef SignFeatureProvider thisType;

    class Feature final : public ContextFeature{
    private:
        override_p_function(Array<String>, DumpData) { return{}; }
    };

    override_p_function(Ref<ContextFeature>, feature){ return new Feature(); }
    override_p_function(Array<String>, DumpData) { return{}; }
};


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


