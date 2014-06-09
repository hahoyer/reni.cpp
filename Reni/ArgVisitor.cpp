#include "Import.h"
#include "ArgVisitor.h"

#include "CodeItem.h"
#include "Result.h"
#include "Type.h"

#include "../HWLib/RefCountContainer.instance.h"

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

p_implementation(ArgVisitor, bool, hasArg){return !results.Find(&Tag::expressionArg).IsEmpty;};
p_implementation(ArgVisitor, bool, hasThis){ return !results.Find(&Tag::expressionThis).IsEmpty; };

ArgVisitor& ArgVisitor::Assign(Tag const* tag, ResultCache const& result)
{
    results.Assign(tag, result.thisRef);
    return *this;
}

Optional<Ref<CodeItem>> const ArgVisitor::Arg(Type const&type, int depth) const{
    auto result = results.Find(&Tag::expressionArg);
    if(result.IsEmpty)
        return {};

    a_is(depth, == , 0);
    a_is(type, == , *result.Value->type);
    return result.Value->code;
};

Optional<Ref<CodeItem>> const ArgVisitor::This(Type const&type, int depth) const{
    auto result = results.Find(&Tag::expressionThis);
    if(result.IsEmpty)
        return{};

    a_is(*type.IndirectType(depth), == , *result.Value->type);
    return result.Value->code;
};
