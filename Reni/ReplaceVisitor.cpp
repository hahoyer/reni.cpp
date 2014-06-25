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

p_implementation(ReplaceVisitor, Array<String>, DumpData){
    return results
        .keys
        .Select<String>([&](External const* key)
    {
        return HWLib::Dump(key) + ": " + HWLib::Dump(results[key]); 
    })
        ->ToArray;
};

p_implementation(ReplaceVisitor, Optional<Externals>, ArgExts)
{
    auto result = results.Find(&External::Arg::Instance);
    if(result.IsEmpty)
        return{};
    return result.Value->exts;
};

p_implementation(ReplaceVisitor, Optional<Externals>, ThisExts)
{
    auto result = results.Find(&External::This::Instance);
    if(result.IsEmpty)
        return{};
    return result.Value->exts;
};

p_implementation(ReplaceVisitor, bool, hasArg){ return !results.Find(&External::Arg::Instance).IsEmpty; };
p_implementation(ReplaceVisitor, bool, hasThis){ return !results.Find(&External::This::Instance).IsEmpty; };


ReplaceVisitor& ReplaceVisitor::Assign(External const&tag, ResultCache const& result)
{
    results.Assign(&tag, result.thisRef);
    return *this;
}

Optional<Ref<CodeItem>> const ReplaceVisitor::Arg(Type const&type, int depth) const{
    auto result = results.Find(&External::Arg::Instance);
    if(result.IsEmpty)
        return {};

    a_is(depth, == , 0);
    a_is(type, == , *result.Value->type);
    return result.Value->code;
}

Optional<Ref<CodeItem>> const ReplaceVisitor::This(Type const&type, int depth) const{
    auto result = results.Find(&External::This::Instance);
    if(result.IsEmpty)
        return{};

    a_is(*type.IndirectType(depth), == , *result.Value->type);
    return result.Value->code;
};
