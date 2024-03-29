#include "Import.h"
#include "ReplaceVisitor.h"

#include "Address.h"
#include "CodeItem.h"
#include "Closure.h"
#include "Result.h"
#include "Type.h"

#include "../HWLib/RefCountContainer.instance.h"

using namespace Reni;
using namespace HWLib;

static bool Trace = true;

HW_PR_IMPL_GETTER(ReplaceVisitor, Array<string>, DumpData){
    return results
        .keys
        .Select<string>([&](External const* key)
    {
        return HWLib::Dump(key) + ": " + HWLib::Dump(results[key]); 
    })
        ->ToArray;
};

Closure ReplaceVisitor::GetClosure(External const& tag) const
{
    auto result = GetResults(tag);
    if (result.IsEmpty)
        return Closure(tag);
    return result.Value->closure;
};

Optional<Ref<CodeItem>> ReplaceVisitor::GetCode(External const& tag) const
{
    auto result = GetResults(tag);
    if (result.IsEmpty)
        return{};
    return result.Value->code;
};

void ReplaceVisitor::SetResults(External const&tag, ResultCache const& result)
{
    results.Assign(&tag, result.thisRef);
    SetDumpString();
}

void ReplaceVisitor::AssumeFunctionArg(External::Function const& tag, Type const& arg) const
{
    auto result = GetResults(tag);
    if (result.IsEmpty)
        return;

    HW_ASSERT_IS(arg.toAddress.data, == , result.Value->type->toAddress.data);
}

Optional<Ref<ResultCache>> ReplaceVisitor::GetResults(External const& tag) const
{
    return results.Find(&tag);
}

void ReplaceVisitor::Assume(External const& tag, Type const& type) const
{
    auto result = GetResults(tag);
    if (result.IsEmpty)
        return ;

    HW_ASSERT(type.toAddress.depth == 0, HW_D_VALUE(type));
    HW_ASSERT_IS(type.size, ==, result.Value->type->toAddress.data);
}

