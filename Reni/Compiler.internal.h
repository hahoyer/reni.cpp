#pragma once
#include "Compiler.h"

using namespace Reni;

class Compiler::internal final
{
    String const _fileName;
    MainTokenFactory tokenFactory;
public:
    internal() = delete;
    internal(internal const&) = delete;

    internal(String const&fileName)
        : _fileName(fileName)
        , sourceCache([&]{return _fileName; })
        , scannerCache([&]{return Scanner(sourceCache.Value, tokenFactory); })
    {
    }

    ValueCache<Source const> sourceCache;
    ValueCache<Scanner const> scannerCache;
};