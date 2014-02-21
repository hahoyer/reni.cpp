#include "Import.h"
#include "RootContext.h"

#include "BitType.h"

using namespace Reni;
static bool Trace = true;

RootContext::RootContext()
: bitTypeInstance(new BitType())
{}


p_implementation(RootContext, BitType const&, bitType){ return *bitTypeInstance; }


override_p_implementation(RootContext, Array<String>, DumpData){
    return{};
};

