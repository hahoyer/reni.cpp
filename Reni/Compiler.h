#pragma once
#include "../HWLib/string.h"
#include "../HWLang/Token.h"
#include "../HWLib/Ref.h"

using namespace HWLib;
using namespace HWLang;

namespace Reni
{
    class CodeBase;
    class CodeItem;
    class Syntax;
    class TokenClass;
    struct ExecutionResult
    {
        int result;
        string data;
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
        Compiler(string const&fileName);
    
        p(Array<Token>, tokens);
        p(Ref<Syntax>, syntax);
        p(CodeBase, code);
        p(Ref<CodeItem>, main);
        p(string, cppCode);
        ExecutionResult Execute();

        static Ref<Syntax> GetSyntaxFromText(string const& text);
    };
}