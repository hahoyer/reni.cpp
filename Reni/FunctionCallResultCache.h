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

        p(CodeFunction, getter);
        p(CodeFunction, setter);
    private:
        p(Optional<WeakRef<Type const>>, arg){ return context.arg; }
        p(ContainerContext const&, container){return context.container;}
        p_function(Array<string>,DumpData) override{return{nd(bodyIndex), nd(context)};}

        ResultData GetResultDataRecursive(Category const& category) const override;
        ResultData GetResultData(Category const& category)const override;
        p(Ref<CodeItem>, codeGet);
        p(Closure, extsGet);
        p(Ref<CodeItem>, CodeSet);
        p(WeakRef<Type>, valueType);
        p(size_t, codeIndex);
        p(FunctionSyntax const&, body);
    };
};