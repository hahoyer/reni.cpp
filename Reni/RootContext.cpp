#include "Import.h"
#include "RootContext.h"

#include "CodeItems.h"
#include "Global.h"
#include "Result.h"
#include "Syntax.h"
#include "../HWLib/RefCountContainer.instance.h"

using namespace Reni;
static bool Trace = true;

class RootContext::internal final
{
public:
    internal()
    {
    };
};


RootContext::RootContext()
    : _internal(new internal)
      , _global(new Global)
{
    SetDumpString();
}

p_implementation(RootContext, Array<string>,DumpData)
{
    return{};
};