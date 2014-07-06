#include "Import.h"
#include "ReplaceVisitor.h"

#include "Address.h"
#include "CodeItem.h"
#include "ContainerContext.h"
#include "External.h"
#include "FunctionCallContext.h"
#include "Syntax.h"
#include "SyntaxContainer.h"
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

Optional<Externals>const ReplaceVisitor::GetExts(External const&tag)const
{
    auto result = GetResults(tag);
    if (result.IsEmpty)
        return{};
    return result.Value->exts;
};

Optional<Ref<CodeItem>>const ReplaceVisitor::GetCode(External const&tag)const
{
    auto result = GetResults(tag);
    if (result.IsEmpty)
        return{};
    return result.Value->code;
};

void ReplaceVisitor::SetResults(External const&tag, ResultCache const& result)
{
    results.Assign(&tag, result.thisRef);
}

void ReplaceVisitor::Assume(External::Function const& tag, FunctionCallContext const& context) const
{
    auto result = GetResults(tag);
    if (result.IsEmpty)
        return;

    a_is(context.arg->toAddress.data, == , result.Value->type->toAddress.data);
}

Optional<Ref<ResultCache>> const ReplaceVisitor::GetResults(External const& tag) const
{
    return results.Find(&tag);
}

void ReplaceVisitor::Assume(External const& tag, Type const& type) const
{
    auto result = GetResults(tag);
    if (result.IsEmpty)
        return ;

    a_if(type.toAddress.depth == 0, nd(type));
    a_is(type.size, ==, result.Value->type->toAddress.data);
}

