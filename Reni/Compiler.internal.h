#pragma once
#include "Compiler.h"

#include "Code.h"
#include "Context.h"
#include "MainTokenFactory.h"
#include "Syntax.h"
#include "TokenClass.h"

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
private:
    ValueCache<Source const> sourceCache;
    Ref<RootContext> rootContext;
public:
    internal() = delete;
    internal(internal const&) = delete;

    internal(String const&fileName)
        : fileName(fileName)
        , sourceCache([=]{return Source::FromFile(fileName); })
        , scannerCache([=]{return Reni::ScannerInstance(sourceCache.Value); })
        , syntaxCache([=]{return GetSyntax(); })
        , codeCache([=]{return GetCode(); })
        , rootContext(new RootContext)
    {}

private:
    Ref<Syntax> const GetSyntax()const{
        auto scannerInstance = scannerCache.Value;
        return Parse<Syntax, TokenClass, Token>(prioTable, scannerInstance);
    };

    Ref<CodeItem> const GetCode()const{
        auto syntax = syntaxCache.Value;
        return syntax->Code(rootContext);
    };

    p(PrioTable, prioTable){
        auto result = PrioTable::CreateLeft({ Any })
            .ParenthesisLevel({ "(", "[", "{" }, { ")", "]", "}" })
            .ParenthesisLevel(Start, End)
            ;
        return result;
    };
};