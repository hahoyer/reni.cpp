#pragma once
#include "Context.h"
#include "Result.h"

#include "../HWLib/WeakPtr.h"

using namespace HWLib;

namespace Reni{
    class ContainerContext;

    class FunctionCallContext final : public Context{
        typedef Context baseType; typedef FunctionCallContext thisType;
    public:
        ContainerContext const& container;
        WeakRef<Type const> const args;
        FunctionCallContext(ContainerContext const& container, WeakRef<Type const> const args)
            : container(container), args(args){}
        ThisRef;
        ResultData const CreateArgReferenceResult(Category category)const;
    private:
        p_function(WeakRef<FunctionCallContext>, functionContext) override{ return thisRef; };
        p_function(WeakRef<Global>, global) override;
        p_function(Array<String>, DumpData) override{ return{nd(args), nd(container)}; }
        p(WeakRef<Type>, objectType);

        SearchResult const GetDefinition(DefineableToken const&token) const override;

    };

};


