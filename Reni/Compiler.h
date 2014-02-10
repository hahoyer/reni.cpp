#pragma once

#include "Scanner.h"
#include "ReniScanner.h"
#include "Token.h"

using namespace HWLib::Match;

namespace Reni
{
    class Compiler final
    {
    public:
        using Scanner = Scanner<MainTokenFactory>;
    private:
        class internal;
        Ref<internal> _internal;
    public:
        Compiler() = delete;
        Compiler(Compiler const&) = delete;
        Compiler(String const&fileName);
    
        p(Ref<Source const>, source);
        p(Ref<Scanner const>, scanner);
    };
}
