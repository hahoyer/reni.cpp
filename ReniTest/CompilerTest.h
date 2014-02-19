#pragma once

using namespace Reni;

namespace _Compiler
{
    class Token;

    class CompilerTest final
    {
        File file;

        CompilerTest(String name)
            : file(name)
            , compiler(name)
        {
        };

    public:
        Compiler compiler;

        CompilerTest()
            : CompilerTest("compilerTest.reni")
        {
        };

        p_mutable(String, text){ return file.Data; }
    };
}
