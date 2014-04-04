#pragma once
#include "Syntax.h"
#include "../HWLang/SourcePart.h"

using namespace HWLib;
using namespace Util;

namespace Reni{

    class FunctionSyntax final : public Syntax{
        typedef Syntax baseType;
        typedef FunctionSyntax thisType;
        Ref<Syntax, true> const left;
        Ref<Syntax, true> const right;
    public:
        FunctionSyntax(Ref<Syntax, true> const left, HWLang::SourcePart const& part, Ref<Syntax, true> const right)
            :baseType(part)
            , left(left)
            , right(right){
            SetDumpString();
        }

        ThisRef;
    private:
        p_function(Array<String>, DumpData)override {
            return{
                nd(left),
                nd(right)
            };
        }

        ResultData const GetResultData(Context const& context, Category category) const override{
            return context.FunctionType(*this)->GetResultData();
        }

    };
}
