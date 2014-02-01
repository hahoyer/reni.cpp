#pragma once
#include "Compiler.h"

using namespace Reni;

class Compiler::internal final
{
    String const _fileName;
public:
    internal() = delete;
    internal(internal const&) = delete;

    internal(String const&fileName)
        : _fileName(fileName)
        , sourceCache([&]{return _fileName; })
    {
    }

    ValueCache<Source const> sourceCache;
};