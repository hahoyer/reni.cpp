#pragma once
#include "CodeItems.h"
#include "Context.h"
#include "Result.h"

using namespace HWLib;

namespace Reni
{
    class ContainerContext;
    class FunctionCallResultCache;

    class FunctionCallContext final : public ChildContext
    {
        using baseType = ChildContext;
        using thisType = FunctionCallContext;
        friend class ContainerContext;
        FunctionCache<Ref<FunctionCallResultCache>, size_t> functionCallResultCache;
    public:
        ContainerContext const& container;
        WeakRef<Type const> const arg;
    private:
        FunctionCallContext(ContainerContext const& container, WeakRef<Type const> const arg);
    public:
        HW_PR_THISREF;
        ResultData const ReferenceResult(Category category, External::Function const& external) const override;
    private:
        HW_PR_DECL_GETTER(WeakRef<FunctionCallContext>, functionContext) override{ return thisRef; };
        HW_PR_DECL_GETTER(Array<string>,DumpData) override{ return{HW_D_VALUE(arg), HW_D_VALUE(container)}; }
        HW_PR_GET(WeakRef<Type>, objectType);
    };


};


