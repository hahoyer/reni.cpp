#pragma once

using namespace HWLib::Match;

namespace Reni
{
    class Compiler
    {
        class internal;
        Ref<internal> _internal;
    public:
        Compiler() = delete;
        Compiler(Compiler const&) = delete;
        Compiler(String const&fileName);
    
        p(Ref<Source const>, source);
    };
}
