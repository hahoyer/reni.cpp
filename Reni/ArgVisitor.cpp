#include "Import.h"
#include "ArgVisitor.h"

#include "Result.h"

using namespace Reni;
using namespace HWLib;

static bool Trace = true;

p_implementation(ArgVisitor, Array<String>, DumpData){
    return results
        .keys
        .Select<String>([&](Tag const* key)
    {
        return HWLib::DumpTypeName(key) + ": " + HWLib::Dump(results[key]); 
    })
        ->ToArray;
}


Ref<CodeItem> const ArgVisitor::Arg(Type const&type) const{
    auto result = results.Find(&Tag::expressionArg);
    if(result.IsEmpty)
        return baseType::Arg(type);

    a_is(type, == , *result->type);
    return result->code;
};

