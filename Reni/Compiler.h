#pragma once

#include "Scanner.h"

using namespace HWLang;

namespace Reni
{
    class Compiler final
    {
    private:
        class internal;
        Pointer<internal> _internal;
    public:
        Compiler() = delete;
        Compiler(Compiler const&) = delete;
        Compiler(String const&fileName);
    
        p(Ref<Source const>, source);
    };

}