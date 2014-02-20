#include "Import.h"
#include "Reni.h"
#include "CompilerTest.h"

static bool Trace = true;

using namespace Reni;
using namespace _Compiler;

void _Reni::RunAll()
{
    CompilerTest ct;
    ct.text = "4711";
    auto s = ct.compiler.syntax;
    d(s);
    b_;
}