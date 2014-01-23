#pragma once

namespace Reni
{
    class Compiler
    {
        class internal;
        internal const & _internal;
    public:
        Compiler() = delete;
        Compiler(Compiler const&) = delete;
        Compiler(String const&fileName);
        ~Compiler();
    };
}
