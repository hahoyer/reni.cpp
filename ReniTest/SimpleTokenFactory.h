#pragma once

namespace _HWLang
{
    class SimpleTokenFactory final
    {
    public:
        template<class TTokenClass>
        class SimpleSyntax final : public DumpableObject
        {
            using baseType = DumpableObject;
            using thisType = SimpleSyntax;
        public:
            using TokenClass = TTokenClass;

            OptRef<thisType const> const left;
            TokenClass const& tokenClass;
            String const name;
            OptRef<thisType const> const right;
            bool const isMatch;

            SimpleSyntax(OptRef<thisType const> const left, TokenClass const& tokenClass, String const&name, OptRef<thisType const> const right, bool isMatch)
                : left(left)
                , tokenClass(tokenClass)
                , name(name)
                , right(right)
                , isMatch(isMatch)
            {
                SetDumpString();
            }
        private:
            override_p_function(Array<String>, DumpData){
                return{
                    nd(left),
                    nd(tokenClass),
                    nd(name),
                    nd(right),
                    nd(isMatch)
                };
            }
            override_p_function(String, DumpShort){
                auto result = name;
                if (left.IsValid)
                    result = "<" + left.DumpShort + ">" + result;
                if (right.IsValid)
                    result += "<" + right.DumpShort + ">";
                if (isMatch)
                    result = "[" + result + "]!";
                return result;
            }
        };

        class TokenClass final : public DumpableObject
        {
            using baseType = DumpableObject;
            using thisType = TokenClass;
        public:
            using Syntax = SimpleSyntax<TokenClass>;

            TokenClass() = default;
            TokenClass(TokenClass const&) = delete;

            Ref<Syntax const> const CreateSyntax(Ref<Syntax const>const left, SourcePart const&part, Ref<Syntax const>const right, bool isMatch)const{
                return new Syntax(left, *this, part, right, isMatch);
            };
            Ref<Syntax const> const CreateSyntax(Ref<Syntax const>const left, SourcePart const&part, bool isMatch)const{
                return new Syntax(left, *this, part, null, isMatch);
            };
            Ref<Syntax const> const CreateSyntax(SourcePart const&part, Ref<Syntax const>const right, bool isMatch)const{
                return new Syntax(null, *this, part, right, isMatch);
            };
            Ref<Syntax const> const CreateSyntax(SourcePart const&part, bool isMatch)const{
                return new Syntax(null, *this, part, null, isMatch);
            };
        private:
            override_p_function(Array<String>, DumpData){ return{}; };
        };

        static TokenClass const& Start;
        static TokenClass const& End;
        static TokenClass const& Number;
        static TokenClass const& Text;
        static TokenClass const& GetTokenClass(String const&name);
        static TokenClass const& GetErrorClass(String const&name);

        class ScannerInstance final : public HWLang::ScannerInstance<HWLang::Token<TokenClass>, SimpleTokenFactory, Reni::Scanner>{
            using baseType = HWLang::ScannerInstance<HWLang::Token<TokenClass>, SimpleTokenFactory, Reni::Scanner>;
            using thisType = ScannerInstance;
        public:
            ScannerInstance(String const&text)
                :baseType(new Source(Source::FromText(text)))
            {};
            ScannerInstance(File const&file)
                :baseType(new Source(Source::FromFile(file.FullName)))
            {};
        };

        using Syntax = TokenClass::Syntax;
    };
}
