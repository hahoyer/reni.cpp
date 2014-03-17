#include "Import.h"
#include "Global.h"

using namespace Reni;
static bool Trace = true;

Global::Global()
: bitType(this)
, voidType(this){
    
}


p_implementation(Global, Array<String>, DumpData){
    return{
        nd(bitType),
        nd(voidType)
    };
};

