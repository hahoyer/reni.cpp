#pragma once
#include "..\HWLang\Token.h"

using namespace HWLib;
using namespace HWLang;

namespace Reni
{
    class CodeItem;
    class Syntax;
    class TokenClass;
    struct ExecutionResult
    {
        int result;
        String data;
    };

    using Token = Token<TokenClass>;

    class Compiler final
    {
    private:
        class internal;
        CtrlRef<internal> _internal;
    public:
        Compiler() = delete;
        Compiler(Compiler const&) = delete;
        Compiler(String const&fileName);
    
        p(Array<Token>, tokens);
        p(Ref<Syntax>, syntax);
        p(CtrlRef<CodeItem>, code);
        p(String, cppCode);
        ExecutionResult const Execute();
    };

}