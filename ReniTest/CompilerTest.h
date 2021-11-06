#pragma once
#include "..\HWLib\File.h"
#include "..\Reni\Compiler.h"

using namespace Reni;
using namespace HWLib;

namespace ReniTest
{
    class Token;

    class CompilerTest final
    {
        File file;
        static string name(){ return  "compilerTest.reni"; };
    public:
        Compiler compiler;

        CompilerTest(string const&text)
            : file(name())
            , compiler(name())
        {
            file.Data = text;
        }

        static void Check(string const& text, string const &expectedResult);
    };
}
