#pragma once
#include "Compiler.h"
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
    PrioTable prioTable;
public:
    ValueCache<Ref<Syntax const>> syntaxCache;
    ValueCache<Reni::ScannerInstance> scannerCache;
private:
    ValueCache<Source const> sourceCache;
public:
    internal() = delete;
    internal(internal const&) = delete;

    internal(String const&fileName)
        : fileName(fileName)
        , sourceCache([=]{return Source::FromFile(fileName); })
        , scannerCache([=]{return Reni::ScannerInstance(sourceCache.Value); })
        , syntaxCache([=]{return GetSyntax(); })
    {}

private:
    Ref<Syntax const> const GetSyntax()const{
        auto scannerInstance = scannerCache.Value;
        return Parse<Syntax const, TokenClass, Token>(prioTable, scannerInstance);
    };
};