#include "Import.h"
#include "Context.h"

#include "Result.h"
#include "Syntax.h"

using namespace Reni;
static bool Trace = true;

static int nextObjectId = 0;


Context::Context()
: baseType(nextObjectId++)
{}

ResultData const Context::GetResultData(Category category, Syntax const&syntax)const{
    md(category, syntax);
    b_;
    return_d(ResultData());
}


RootContext::RootContext()
{}

override_p_implementation(RootContext, Array<String>, DumpData){
    return{};
};

