#pragma once
#include "Compiler.h"

#include "CodeVisitor.h"
#include "CppCompilerScripting.h"
#include "MainTokenFactory.h"
#include "RootContext.h"
#include "Scanner.h"
#include "Syntax.h"
#include "TokenClass.h"
#include "../HWLib/ValueCache.h"
#include "../HWLang/ScannerInstance.h"
#include "../HWLang/PrioParser.h"

using namespace HWLib;
using namespace HWLang;

namespace Reni{

    class ScannerInstance final : public HWLang::ScannerInstance<Token, MainTokenFactory, Scanner>{
        using baseType = HWLang::ScannerInstance<Token, MainTokenFactory, Scanner>;
        using thisType = ScannerInstance;
    public:
        ScannerInstance(Source const& source)
            :baseType(source)
        {};
    };
}

using namespace Reni;


class Compiler::internal final
{
    String const fileName;
public:
    ValueCache<Ref<CodeItem>> codeCache;
    ValueCache<Ref<Syntax>> syntaxCache;
    ValueCache<String> cppCodeCache;
private:
    Ref<RootContext> rootContext;
public:
    internal() = delete;
    internal(internal const&) = delete;

    internal(String const&fileName)
        : fileName  (fileName)
        , rootContext(new RootContext)
        , syntaxCache([&]{return GetSyntax(); })
        , codeCache  ([&]{return GetCode(); })
        , cppCodeCache([&]{return GetCppCode(); })
    {}

    ExecutionResult const Execute(){
        CppCompilerScripting ccs = cppCodeCache.Value;
        ccs.Execute();
        return ExecutionResult{ ccs.result, ccs.output };

    }

    static Ref<Syntax> const GetSyntaxFromText(String const& text){
        return GetSyntax(*Source::CreateFromText(text));
    };

private:
    Ref<Syntax> const GetSyntax()const{
        auto source = *Source::CreateFromFile(fileName);
        auto scannerInstance = Reni::ScannerInstance(source);
        return Parse<Ref<Syntax>, Ref<Syntax,true>, TokenClass, Token>(prioTable(), scannerInstance);
    };

    static Ref<Syntax> const GetSyntaxFromFIle(String const& file){
        return GetSyntax(*Source::CreateFromFile(file));
    };

    static Ref<Syntax> const GetSyntax(Source const&source){
        auto scannerInstance = Reni::ScannerInstance(source);
        return Parse<Ref<Syntax>, Ref<Syntax, true>, TokenClass, Token>(prioTable(), scannerInstance);
    };

    Ref<CodeItem> const GetCode()const{
        auto syntax = syntaxCache.Value;
        return syntax->Code(*rootContext);
    };

    static PrioTable const prioTable(){
        return
            HWLang::PrioTable::CreateLeft({ Any })
            .Left({"+", "-"})
            .ParenthesisLevel({ "(", "[", "{" }, { ")", "]", "}" })
            .ParenthesisLevel(Start, End)
            ;
    };

    class CodeVisitor : public Reni::CodeVisitor
    {
        override_p_function(Array<String>, DumpData){ return{}; };
        virtual String const Const(Size const size, BitsConst const& value) const override;

    };

    String const GetCppCode()const{
        CodeVisitor visitor;
        return CppCompilerScripting(codeCache.Value->ToCpp(visitor)).program;
    };

};


