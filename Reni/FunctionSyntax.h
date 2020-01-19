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
        Optional<Ref<Syntax>> const setter;
        Optional<Ref<Syntax>> const getter;

        FunctionSyntax(Optional<Ref<Syntax>> const setter, SourcePart const& part, Optional<Ref<Syntax>> const getter)
            :baseType(part)
             , setter(setter)
             , getter(getter)
        {
            SetDumpString();
        }

        ThisRef;

    private:
        p_function(String, SmartDump) override; 
        p_function(int, priority) override{ return 0; }

        ResultData const GetResultData(Context const& context, Category category) const override;
    };

}
