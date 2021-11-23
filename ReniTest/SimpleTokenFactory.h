#pragma once
#include "../HWLib/DumpableObject.h"
#include "../HWLib/DumpToString.h"
#include "../HWLang/SourcePart.h"
#include "../HWLang/ScannerInstance.h"
#include "../HWLang/Token.h"
#include "../HWLib/File.h"
#include "../HWLib/RefCountProvider.h"
#include "../Util/Scanner.h"

using namespace HWLib;
using namespace HWLang;

namespace TestHWLang{
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

            Optional<Ref<thisType>> const left;
            TokenClass const& tokenClass;
            string const name;
            Optional<Ref<thisType>> const right;
            bool const isMatch;

            SimpleSyntax(Optional<Ref<thisType>> const left, TokenClass const& tokenClass, string const&name, Optional<Ref<thisType>> const right, bool isMatch)
                : left(left)
                  , tokenClass(tokenClass)
                  , name(name)
                  , right(right)
                  , isMatch(isMatch){
                SetDumpString();
            }

        private:
            p_function(Array<string>,DumpData) override{
                return{
                    HW_D_VALUE(left),
                    HW_D_VALUE(tokenClass),
                    HW_D_VALUE(name),
                    HW_D_VALUE(right),
                    HW_D_VALUE(isMatch)
                };
            }

            p_function(string,DumpShort) override{
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

            p(bool, AcceptsMatch){ return this->isMatch; }

            Ref<Syntax> const Mismatch(Optional<Ref<Syntax>>const left, SourcePart const&part, Optional<Ref<Syntax>>const right)const{
                return new Syntax(left, *this, part, right, !isMatch);
            };
            Ref<Syntax> const CreateSyntax(Optional<Ref<Syntax>>const left, SourcePart const&part, Optional<Ref<Syntax>>const right)const{
                return new Syntax(left, *this, part, right, isMatch);
            };

        private:
            p_function(Array<string>,DumpData) override{
                return{};
            };
        };

        static TokenClass const& Start;
        static TokenClass const& End;
        static TokenClass const& Number;
        static TokenClass const& Text;
        static TokenClass const& GetTokenClass(string const&name);
        static TokenClass const& GetErrorClass(string const&name);

        class ScannerInstance final : public HWLang::ScannerInstance<HWLang::Token<TokenClass>, SimpleTokenFactory, Util::Scanner>{
            using baseType = HWLang::ScannerInstance<HWLang::Token<TokenClass>, SimpleTokenFactory, Util::Scanner>;
            using thisType = ScannerInstance;
        public:

            ScannerInstance(string const&text)
                :baseType(Source::FromText(text)){
                SetDumpString();
            };

            ScannerInstance(File const&file)
                :baseType(Source::FromFile(file.FullName)){
                SetDumpString();
            };
        };

        using Syntax = TokenClass::Syntax;
    };
}