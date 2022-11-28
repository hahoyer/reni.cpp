#pragma once

#include "ContainerContext.h"
#include "FunctionCallContext.h"
#include "FunctionSyntax.h"
#include "Global.h"
#include "SyntaxContainer.h"
#include "Type.h"
#include "../HWLib/ValueCache.h"
#include "../HWLib/FunctionCache.h"

using namespace HWLib;

namespace Reni
{
    class CodeFunction;
    class FunctionCallResultCache final : public ResultCache
    {
        typedef ResultCache baseType;
        typedef FunctionCallResultCache thisType;
        size_t const bodyIndex;
        FunctionCallContext const& context;
    public:
        Global::Function function;

        FunctionCallResultCache(FunctionCallContext const& context, size_t bodyIndex);

        HW_PR_GET(CodeFunction, getter);
        HW_PR_GET(CodeFunction, setter);
    private:
        HW_PR_GET(Optional<WeakRef<Type const>>, arg){ return context.arg; }
        HW_PR_GET(ContainerContext const&, container){return context.container;}
        HW_PR_DECL_GETTER(Array<string>,DumpData) override{return{HW_D_VALUE(bodyIndex), HW_D_VALUE(context)};}

        ResultData GetResultDataRecursive(Category const& category) const override;
        ResultData GetResultData(Category const& category)const override;
        HW_PR_GET(Ref<CodeItem>, codeGet);
        HW_PR_GET(Closure, extsGet);
        HW_PR_GET(Ref<CodeItem>, CodeSet);
        HW_PR_GET(WeakRef<Type>, valueType);
        HW_PR_GET(size_t, codeIndex);
        HW_PR_GET(FunctionSyntax const&, body);
    };
};