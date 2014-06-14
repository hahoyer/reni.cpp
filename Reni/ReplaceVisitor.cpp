#include "Import.h"
#include "ReplaceVisitor.h"

#include "CodeItem.h"
#include "External.h"
#include "Result.h"
#include "Type.h"

#include "../HWLib/DumpMacros.h"
#include "../HWLib/RefCountContainer.instance.h"

using namespace Reni;
using namespace HWLib;

static bool Trace = true;

ReplaceVisitor::Tag ReplaceVisitor::Tag::expressionThis;
ReplaceVisitor::Tag ReplaceVisitor::Tag::expressionArg;


p_implementation(ReplaceVisitor, Array<String>, DumpData){
    return results
        .keys
        .Select<String>([&](Tag const* key)
    {
        return HWLib::Dump(key) + ": " + HWLib::Dump(results[key]); 
    })
        ->ToArray;
};

p_implementation(ReplaceVisitor, Optional<Externals>, ArgExts)
{
    auto result = results.Find(&Tag::expressionArg);
    if(result.IsEmpty)
        return{};
    return result.Value->exts;
};

p_implementation(ReplaceVisitor, Optional<Externals>, ThisExts)
{
    auto result = results.Find(&Tag::expressionThis);
    if(result.IsEmpty)
        return{};
    return result.Value->exts;
};

p_implementation(ReplaceVisitor, bool, hasArg){ return !results.Find(&Tag::expressionArg).IsEmpty; };
p_implementation(ReplaceVisitor, bool, hasThis){ return !results.Find(&Tag::expressionThis).IsEmpty; };


ReplaceVisitor& ReplaceVisitor::Assign(Tag const* tag, ResultCache const& result)
{
    results.Assign(tag, result.thisRef);
    return *this;
}

Optional<Ref<CodeItem>> const ReplaceVisitor::Arg(Type const&type, int depth) const{
    auto result = results.Find(&Tag::expressionArg);
    if(result.IsEmpty)
        return {};

    a_is(depth, == , 0);
    a_is(type, == , *result.Value->type);
    return result.Value->code;
}

Optional<Ref<CodeItem>> const ReplaceVisitor::This(Type const&type, int depth) const{
    auto result = results.Find(&Tag::expressionThis);
    if(result.IsEmpty)
        return{};

    a_is(*type.IndirectType(depth), == , *result.Value->type);
    return result.Value->code;
};
