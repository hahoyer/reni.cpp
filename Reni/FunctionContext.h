#pragma once
#include "Context.h"
#include "Result.h"

#include "../HWLib/WeakPtr.h"

using namespace HWLib;

namespace Reni
{
    class ContainerContext;

    class FunctionCallContext final : public ChildContext
    {
        using baseType = ChildContext;
        using thisType = FunctionCallContext;
        static int nextIndex;
        int const index;
    public:
        ContainerContext const& container;
        WeakRef<Type const> const args;

        FunctionCallContext(ContainerContext const& container, WeakRef<Type const> const args);

        ThisRef;
        ResultData const CreateArgReferenceResult(Category category)const;
        Ref<CodeItem> const CallGetterCode(Type const& resultType) const;
    private:
        p_function(WeakRef<FunctionCallContext>, functionContext) override{ return thisRef; };
        p_function(Array<String>, DumpData) override{ return{nd(args), nd(container), nd(index)}; }
        p(WeakRef<Type>, objectType);
    };

};


