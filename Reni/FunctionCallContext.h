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
        FunctionCache<Ref<FunctionCallResultCache>, int> functionCallResultCache;
    public:
        ContainerContext const& container;
        WeakRef<Type const> const arg;
    private:
        FunctionCallContext(ContainerContext const& container, WeakRef<Type const> const arg);
    public:
        ThisRef;
        ResultData const ReferenceResult(Category category, External::Function const& external) const override;
    private:
        p_function(WeakRef<FunctionCallContext>, functionContext) override{ return thisRef; };
        p_function(Array<String>, DumpData) override{ return{nd(arg), nd(container)}; }
        p(WeakRef<Type>, objectType);
    };


};


