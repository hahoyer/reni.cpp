#pragma once
#include "../HWLib/DumpableObject.h"
#include "CodeVisitor.h"
#include "Global.h"

using namespace HWLib;
using namespace HWLang;

namespace Reni
{
    class CodeBase final : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = CodeBase;

    public:
        Ref<CodeItem> const main;
        Array<Global::Function> const functions;

        CodeBase(Ref<CodeItem> const& main, Array<Global::Function> const& functions)
            : main(main), functions(functions)
        {
            SetDumpString();
        }

        p(String, cppMain);
        p(String, cppFunctions);
    private:
        p_function(Array<String>, DumpData) override
        {
            return{nd(main), nd(functions)};
        }
    };
};