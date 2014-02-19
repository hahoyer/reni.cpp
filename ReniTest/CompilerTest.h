#pragma once

using namespace Reni;

namespace _Compiler
{
    class Token;

    class CompilerTest final
    {
        File file;
        Compiler compiler;

        CompilerTest(String name)
            : file(name)
            , compiler(name)
        {
        };

    public:
        CompilerTest()
            : CompilerTest("compilerTest.reni")
        {
        };

        p_mutable(String, text){ return file.Data; }
    };
}
