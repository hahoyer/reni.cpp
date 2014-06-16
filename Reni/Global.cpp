#include "Import.h"
#include "Global.h"

using namespace Reni;
static bool Trace = true;

int Global::nextIndex = 0;

Global::Global()
: bitType(this)
, voidType(this)
, codeIndexCache([&](FunctionCallResultCache const*){return nextIndex++; })
{
}

int const Global::CodeIndex(FunctionCallResultCache const& target) const
{
    return codeIndexCache(&target);
}

p_implementation(Global, Array<String>, DumpData){
    return{
        nd(bitType),
        nd(voidType)
    };
};

