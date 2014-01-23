#pragma once
#include "Compiler.h"
#include "Source.h"

using namespace Reni;

class Compiler::internal
{
    String const _fileName;
public:
    internal() = delete;
    internal(internal const&) = delete;

    internal(String const&fileName)
        : _fileName(fileName)
        , SourceCache([&]{return _fileName; })
    {
    }

    ValueCache<Source> SourceCache;
};