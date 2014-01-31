#include "Import.h"

#include "Reni.h"

static bool Trace = true;

using namespace _Reni;
using namespace Reni;

void _Reni::Split()
{
    Compiler c = __FILE__;
    auto s = c.source;
    auto tf = MainTokenFactory();
    Scanner<ReniScanner,MainTokenFactory,Token> scanner (s,tf);

    auto ss = scanner.Step();
}
