#pragma once

namespace _HWLang
{
    template<class TTokenClass>
    class Syntax : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = Syntax;
    public:
        using TokenClass = TTokenClass;

        OptRef<Syntax const> const left;
        TokenClass const& tokenClass;
        String const name;
        OptRef<Syntax const> const right;

        Syntax(OptRef<Syntax const> const left, TokenClass const& tokenClass, String const&name, OptRef<Syntax const> const right)
            : left(left)
            , tokenClass(tokenClass)
            , name(name)
            , right(right)
        {
            SetDumpString();
        }
    private:
        virtual p_function(Array<String>, DumpData)override{
            return{
                nd(left),
                nd(tokenClass),
                nd(name),
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

        TokenClass() = default;
        TokenClass(TokenClass const&) = delete;
    
        Ref<Syntax const> const CreateSyntax(Ref<Syntax const>const left, SourcePart const&part, Ref<Syntax const>const right)const{
            return new Syntax(left, *this, part, right);
        };
        Ref<Syntax const> const CreateSyntax(Ref<Syntax const>const left, SourcePart const&part)const{
            return new Syntax(left, *this, part, null);
        };
        Ref<Syntax const> const CreateSyntax(SourcePart const&part, Ref<Syntax const>const right)const{
            return new Syntax(null, *this, part, right);
        };
        Ref<Syntax const> const CreateSyntax(SourcePart const&part)const{
            return new Syntax(null, *this, part, null);
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
