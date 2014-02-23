#pragma once

using namespace HWLang;

namespace Reni
{
    class CodeItem;
    class Syntax;
    class TokenClass;

    using Token = HWLang::Token<TokenClass>;

    class Compiler final
    {
    private:
        class internal;
        Pointer<internal> _internal;
    public:
        Compiler() = delete;
        Compiler(Compiler const&) = delete;
        Compiler(String const&fileName);
    
        p(Array<Token>, tokens);
        p(Ref<Syntax>, syntax);
        p(Ref<CodeItem>, code);
        p(String, cppCode);
        void Execute();
    };

}