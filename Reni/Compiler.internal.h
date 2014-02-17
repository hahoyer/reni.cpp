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
        , source(Source::FromFile(_fileName))
    {
    }

    Source const sourceCache;
    ValueCache<Syntax const> syntaxCache;

private:
    Ref<Source const> const GetSource()const{
        Source::FromFile(_fileName);
    }

};