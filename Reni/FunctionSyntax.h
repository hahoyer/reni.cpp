#pragma once
#include "Syntax.h"
#include "../HWLang/SourcePart.h"

using namespace HWLib;
using namespace Util;

namespace Reni{

    class FunctionSyntax final : public Syntax{
        typedef Syntax baseType;
        typedef FunctionSyntax thisType;
    public:
        Ref<Syntax, true> const setter;
        Ref<Syntax, true> const getter;
        FunctionSyntax(Ref<Syntax, true> const setter, SourcePart const& part, Ref<Syntax, true> const getter)
            :baseType(part)
            , setter(setter)
            , getter(getter){
            SetDumpString();
        }

        ThisRef;
    private:
        p_function(Array<String>, DumpData)override {
            return{
                nd(setter),
                nd(getter)
            };
        }

        ResultData const GetResultData(Context const& context, Category category) const override{
            return context.FunctionType(*this)->GetResultData();
        }

    };
}
