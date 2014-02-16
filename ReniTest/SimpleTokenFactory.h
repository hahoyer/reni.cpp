#pragma once

namespace _Compiler
{
    template<class TTokenClass>
    class Syntax : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = Syntax;
    public:
        using TokenClass = TTokenClass;
    private:
        OptRef<Syntax const> const left;
        TokenClass const& tokenClass;
        OptRef<Syntax const> const right;
    public:
        Syntax(OptRef<Syntax const> const left, TokenClass const& tokenClass, OptRef<Syntax const> const right)
            : left(left)
            , tokenClass(tokenClass)
            , right(right)
        {
            SetDumpString();
        }
    private:
        virtual p_function(Array<String>, DumpData)override{
            return{
                nd(left),
                nd(tokenClass),
                nd(right)
            };
        }
    };


    class TokenClass final: public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = TokenClass;
    public:
        using Syntax = Syntax<TokenClass>;
    
        Ref<Syntax const> const CreateSyntax(Ref<Syntax const>const left, SourcePart part, Ref<Syntax const>const right)const{
            return new Syntax(left, *this, right);
        };
        Ref<Syntax const> const CreateSyntax(Ref<Syntax const>const left, SourcePart part)const{
            return new Syntax(left, *this, null);
        };
        Ref<Syntax const> const CreateSyntax(SourcePart part, Ref<Syntax const>const right)const{
            return new Syntax(null, *this, right);
        };
        Ref<Syntax const> const CreateSyntax(SourcePart part)const{
            return new Syntax(null, *this, null);
        };
    private:
        virtual p_function(Array<String>, DumpData)override{ return{}; };
    };

    class TokenFactory final
    {
    public:
        static TokenClass const& Start;
        static TokenClass const& End;
        static TokenClass const& Number;
        static TokenClass const& Text;
        static TokenClass const& GetTokenClass(String const&name);
        static TokenClass const& GetError(String const&name);
    };

    using ScannerInstance = HWLang::ScannerInstance<HWLang::Token<TokenClass>, TokenFactory, Reni::Scanner>;

}
