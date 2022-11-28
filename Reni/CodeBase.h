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

        CodeBase(Ref<CodeItem> const& main, Array<Global::Function> const& functions);

        HW_PR_GET(string, cppMain);
        HW_PR_GET(string, cppFunctions);
        HW_PR_GET(string, cppDeclarations);
    private:
        HW_PR_DECL_GETTER(Array<string>,DumpData) override
        {
            return{HW_D_VALUE(main), HW_D_VALUE(functions)};
        }
    };
};