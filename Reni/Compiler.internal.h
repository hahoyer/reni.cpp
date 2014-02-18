#pragma once
#include "Compiler.h"
#include "MainTokenFactory.h"
#include "Syntax.h"

namespace Reni{
    class ScannerInstance final : public HWLang::ScannerInstance<Token<TokenClass>, MainTokenFactory, Scanner>{
        using baseType = HWLang::ScannerInstance<Token<TokenClass>, MainTokenFactory, Scanner>;
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
    ValueCache<Source const> sourceCache;
private:
    ValueCache<Reni::ScannerInstance> scannerCache;
    ValueCache<Ref<Syntax const>> syntaxCache;
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
        return Parse<Syntax>(prioTable, scannerInstance);
    };
};