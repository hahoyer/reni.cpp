#include "Import.h"
#include "ArgVisitor.h"

#include "Result.h"

using namespace Reni;
using namespace HWLib;

static bool Trace = true;


ArgVisitor::ArgVisitor(ResultCache const& result):result(result){
    SetDumpString();
}

Ref<CodeItem> const ArgVisitor::Arg(Type const&type) const{
    a_is(type, == , *result.type);
    return result.code;
};
