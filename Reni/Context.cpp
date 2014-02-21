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
    return syntax.GetResultData(*this, category);
}

pure_p_implementation(Context, RootContext const&, rootContext);
