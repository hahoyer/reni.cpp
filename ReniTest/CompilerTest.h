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
        static String const name(){ return  "compilerTest.reni"; };
    public:
        Compiler compiler;

        CompilerTest(String text)
            : file(name())
            , compiler(name())
        {
            file.Data = text;
        }

        static void Check(String const& text, String const expectedResult);
    };
}
