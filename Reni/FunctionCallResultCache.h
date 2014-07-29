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
        int const bodyIndex;
        FunctionCallContext const& context;
    public:
        Global::Function function;

        FunctionCallResultCache(FunctionCallContext const& context, int bodyIndex);

        p(CodeFunction, getter);
        p(CodeFunction, setter);
    private:
        p(Optional<WeakRef<Type const>>, arg){ return context.arg; }
        p(ContainerContext const&, container){return context.container;}
        p_function(Array<String>, DumpData) override{return{nd(bodyIndex), nd(context)};}

        ResultData const GetResultData(Category category)const override;
        p(Ref<CodeItem>, codeGet);
        p(Externals, extsGet);
        p(Ref<CodeItem>, CodeSet);
        p(WeakRef<Type>, valueType);
        p(ResultData, valueInRecursion);
        p(int, codeIndex);
        p(FunctionSyntax const&, body);
    };
};