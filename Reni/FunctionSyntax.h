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

        HW_PR_THISREF;

    private:
        HW_PR_DECL_GETTER(string, SmartDump) override; 
        HW_PR_DECL_GETTER(int, priority) override{ return 0; }

        ResultData GetResultData(const Context& context, Category const& category) const override;
    };

}
