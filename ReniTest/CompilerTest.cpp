#include "Import.h"
#include "CompilerTest.h"

using namespace _Reni;
using namespace Reni;
using namespace _Compiler;

static bool Trace = true;


p_mutator_implementation(CompilerTest, String, Text){ _file.Data = value; };

p_implementation(CompilerTest, Array<Token>, ToArray)
{
    return Compiler::Scanner(*_compiler.scanner).ToArray();
};

