#pragma once

#include "ContainerContext.h"
#include "FunctionCallContext.h"
#include "FunctionSyntax.h"
#include "SyntaxContainer.h"
#include "Type.h"
#include "../HWLib/ValueCache.h"
#include "../HWLib/FunctionCache.h"

namespace Reni
{
    class FunctionCallResultCache final : public ResultCache
    {
        typedef ResultCache baseType;
        typedef FunctionCallResultCache thisType;
        int const bodyIndex;
        FunctionCallContext const& context;

    public:
        FunctionCallResultCache(FunctionCallContext const& context, int bodyIndex)
            : context(context)
            , bodyIndex(bodyIndex)
        {
            SetDumpString();
        }

    private:
        p(WeakPtr<Type const>, args){return context.args;}
        p(ContainerContext const&, container){return context.container;}
        p_function(Array<String>, DumpData) override{return{nd(bodyIndex), nd(context)};}

        ResultData const GetResultData(Category category)const override;
        p(Ref<CodeItem>, codeGet);
        p(Ref<CodeItem>, CodeSet);
        p(WeakRef<Type>, valueType);
        p(WeakRef<Type>, valueTypeInRecursion);
        p(Ref<CodeItem>, codeGetter);
        p(Ref<CodeItem>, CodeSetter);
        p(int, codeIndex);
        p(FunctionSyntax const&, body);
    };
};