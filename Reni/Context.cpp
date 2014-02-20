#include "Import.h"
#include "Context.h"

using namespace Reni;
static bool Trace = true;

static int nextObjectId = 0;


Context::Context()
: baseType(nextObjectId++)
{}


RootContext::RootContext()
{}


override_p_implementation(RootContext, Array<String>, DumpData){
    return{};
};

