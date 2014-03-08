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
                    result = "<" + HWLib::DumpShort(left) + ">" + result;
                if (right.IsValid)
                    result += "<" + HWLib::DumpShort(right) + ">";
                if (isMatch)
                    result = "[" + result + "]!";
                return result;
            }
        };

        class TokenClass final : public DumpableObject{
            using baseType = DumpableObject;
            using thisType = TokenClass;
        public:
            typedef SimpleSyntax<TokenClass> Syntax;

            //using Syntax = SimpleSyntax<TokenClass>;

            TokenClass() = default;
            TokenClass(TokenClass const&) = delete;

            Ref<Syntax> const CreateSyntax(Ref<Syntax>const left, SourcePart const&part, Ref<Syntax>const right, bool isMatch)const{
                return new Syntax(left, *this, part, right, isMatch);
            };

            Ref<Syntax> const CreateSyntax(Ref<Syntax>const left, SourcePart const&part, bool isMatch)const{
                return new Syntax(left, *this, part, {}, isMatch);
            };

            Ref<Syntax> const CreateSyntax(SourcePart const&part, Ref<Syntax>const right, bool isMatch)const{
                return new Syntax({}, *this, part, right, isMatch);
            };

            Ref<Syntax> const CreateSyntax(SourcePart const&part, bool isMatch)const{
                return new Syntax({}, *this, part, {}, isMatch);
            };

        private:
            override_p_function(Array<String>, DumpData){
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