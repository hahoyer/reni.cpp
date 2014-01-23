#pragma once

namespace Reni
{
    class Source;
    class Compiler
    {
        class internal;
        Ref<internal> _internal;
    public:
        Compiler() = delete;
        Compiler(Compiler const&) = delete;
        Compiler(String const&fileName);
    
        p(Source, source);
    };
}
