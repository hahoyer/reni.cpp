#include "Import.h"
#include "ArgVisitor.h"

#include "Result.h"

using namespace Reni;
using namespace HWLib;

static bool Trace = true;

ArgVisitor::Tag ArgVisitor::Tag::expressionThis;
ArgVisitor::Tag ArgVisitor::Tag::expressionArg;


p_implementation(ArgVisitor, Array<String>, DumpData){
    return results
        .keys
        .Select<String>([&](Tag const* key)
    {
        return HWLib::Dump(key) + ": " + HWLib::Dump(results[key]); 
    })
        ->ToArray;
};

Ref<CodeItem, true> const ArgVisitor::Arg(Type const&type, int depth) const{
    auto result = results.Find(&Tag::expressionArg);
    if(result.IsEmpty)
        return {};

    a_is(depth, == , 0);
    a_is(type, == , *result->type);
    return result->code;
};

Ref<CodeItem, true> const ArgVisitor::This(Type const&type, int depth) const{
    auto result = results.Find(&Tag::expressionThis);
    if(result.IsEmpty)
        return{};

    a_is(*type.IndirectType(depth), == , *result->type);
    return result->code;
};
