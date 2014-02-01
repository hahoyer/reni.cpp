#pragma once

using namespace Reni;
namespace _Reni
{

    namespace _Compiler
    {

        class CompilerTest final
        {
            File _file;
            Compiler _compiler;

            CompilerTest(String name)
                : _file(name)
                , _compiler(name)
            {
            };

        public:
            CompilerTest()
                : CompilerTest("compilerTest.reni")
            {
            };

            p_mutable(String, Text){ return _file.Data; }
            p(Array<Token>, ToArray);
        };
    }
}