#include "Import.h"
#include "ArgVisitor.h"

#include "Result.h"

using namespace Reni;
using namespace HWLib;

static bool Trace = true;


ArgVisitor::ArgVisitor(ResultCache const& result):result(result){
    SetDumpString();
};
