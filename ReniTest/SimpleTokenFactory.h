#pragma once
#include "../HWLib/DumpableObject.h"
#include "../HWLib/DumpToString.h"
#include "../HWLang/SourcePart.h"
#include "../HWLang/ScannerInstance.h"
#include "../HWLang/Token.h"
#include "../Reni/Scanner.h"
#include "../HWLib/File.h"
#include "../HWLib/RefCountProvider.h"

using namespace HWLib;
using namespace HWLang;

namespace _HWLang{
    class SimpleTokenFactory final{
        typedef RefCountProvider baseType;
        typedef SimpleTokenFactory thisType;
    public:

        template<class TTokenClass>
        class SimpleSyntax final 
            : public DumpableObject
            , public RefCountProvider{
            using baseType = DumpableObject;
            using thisType = SimpleSyntax;
        public:
            typedef TTokenClass TokenClass;

            Ref<thisType, true> const left;
            TokenClass const& tokenClass;
            String const name;
            Ref<thisType, true> const right;
            bool const isMatch;

            SimpleSyntax(Ref<thisType, true> const left, TokenClass const& tokenClass, String const&name, Ref<thisType, true> const right, bool isMatch)
                : left(left)
                  , tokenClass(tokenClass)
                  , name(name)
                  , right(right)
                  , isMatch(isMatch){
                SetDumpString();
            }

        private:
            p_function(Array<String>,DumpData) override{
                return{
                    nd(left),
                    nd(tokenClass),
                    nd(name),
                    nd(right),
                    nd(isMatch)
                };
            }

            p_function(String,DumpShort) override{
                auto result = name;
                if(!left.IsEmpty)
                    result = "<" + HWLib::DumpShort(left) + ">" + result;
                if(!right.IsEmpty)
                    result += "<" + HWLib::DumpShort(right) + ">";
                if (isMatch)
                    result = "[" + result + "]!";
                return result;
            }
        };

        class TokenClass final : public DumpableObject{
            using baseType = DumpableObject;
            using thisType = TokenClass;
            bool const isMatch;
        public:
            typedef SimpleSyntax<TokenClass> Syntax;

            //using Syntax = SimpleSyntax<TokenClass>;

            TokenClass() = default;
            TokenClass(TokenClass const&) = delete;
            TokenClass(bool isMatch):isMatch(isMatch){};

            bool AcceptsMatch(bool isMatch) const{ return isMatch == this->isMatch; }
            virtual_p(Optional<bool>, HasLeft){ return{}; }
            virtual_p(Optional<bool>, HasRight){ return{}; }

            Ref<Syntax> const Mismatch(Ref<Syntax, true>const left, SourcePart const&part, Ref<Syntax, true>const right)const{
                return new Syntax(left, *this, part, right, !isMatch);
            };
            Ref<Syntax> const Infix(Ref<Syntax >const left, SourcePart const&part, Ref<Syntax      >const right)const{
                return new Syntax(left, *this, part, right, isMatch);
            };
            Ref<Syntax> const Suffix(Ref<Syntax >const left, SourcePart const&part)const{
                return new Syntax(left, *this, part, {}, isMatch);
            };
            Ref<Syntax> const Sufanyfix(Ref<Syntax >const left, SourcePart const&part, Ref<Syntax, true>const right)const{
                return new Syntax(left, *this, part, right, isMatch);
            };

            Ref<Syntax> const Prefix(SourcePart const&part, Ref<Syntax      >const right)const{
                return new Syntax({}, *this, part, right, isMatch);
            };
            Ref<Syntax> const Terminal(SourcePart const&part)const{
                return new Syntax({}, *this, part, {}, isMatch);
            };
            Ref<Syntax> const NoSuffix(SourcePart const&part, Ref<Syntax, true>const right)const{
                return new Syntax({}, *this, part, right, isMatch);
            };

            Ref<Syntax> const Preanyfix(Ref<Syntax, true>const left, SourcePart const&part, Ref<Syntax    >const right)const{
                return new Syntax(left, *this, part, right, isMatch);
            };
            Ref<Syntax> const NoPrefix(Ref<Syntax, true>const left, SourcePart const&part)const{
                return new Syntax(left, *this, part, {}, isMatch);
            };
            Ref<Syntax> const Anyfix(Ref<Syntax, true>const left, SourcePart const&part, Ref<Syntax, true>const right)const{
                return new Syntax(left, *this, part, right, isMatch);
            };

        private:
            p_function(Array<String>,DumpData) override{
                return{};
            };
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
                :baseType(Source::FromText(text)){
            };

            ScannerInstance(File const&file)
                :baseType(Source::FromFile(file.FullName)){
            };
        };

        using Syntax = TokenClass::Syntax;
    };
}