#pragma once
#include "Compiler.h"

#include "CodeView.h"
#include "MainTokenFactory.h"
#include "RootContext.h"
#include "Scanner.h"
#include "Syntax.h"
#include "TokenClass.h"

static bool Trace = true;

namespace Reni{

    class ScannerInstance final : public HWLang::ScannerInstance<Token, MainTokenFactory, Scanner>{
        using baseType = HWLang::ScannerInstance<Token, MainTokenFactory, Scanner>;
        using thisType = ScannerInstance;
    public:
        ScannerInstance(Ref<Source const> source)
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
    ValueCache<Reni::ScannerInstance> scannerCache;
    ValueCache<Ref<Syntax>> syntaxCache;
    ValueCache<String> cppCodeCache;
private:
    ValueCache<Source const> sourceCache;
    Ref<RootContext> rootContext;
public:
    internal() = delete;
    internal(internal const&) = delete;

    internal(String const&fileName)
        : fileName  (fileName)
        , rootContext(new RootContext)
        , sourceCache([=]{return Source::FromFile(fileName); })
        , scannerCache([=]{return Reni::ScannerInstance(sourceCache.Value); })
        , syntaxCache([=]{return GetSyntax(); })
        , codeCache  ([=]{return GetCode(); })
        , cppCodeCache([=]{return GetCppCode(); })
    {}

    void Execute(){
        auto s = cppCodeCache.Value;
        dd(s);
        CodeView(s).Execute();
    }

private:
    Ref<Syntax> const GetSyntax()const{
        auto scannerInstance = scannerCache.Value;
        return Parse<Syntax, TokenClass, Token>(prioTable, scannerInstance);
    };

    Ref<CodeItem> const GetCode()const{
        auto syntax = syntaxCache.Value;
        return syntax->Code(*rootContext);
    };

    p(PrioTable, prioTable){
        return
            PrioTable::CreateLeft({ Any })
            .ParenthesisLevel({ "(", "[", "{" }, { ")", "]", "}" })
            .ParenthesisLevel(Start, End)
            ;
    };

    String const GetCppCode()const{
        return CodeView(codeCache.Value->ToCpp).program;
    };

};