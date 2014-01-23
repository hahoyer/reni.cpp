#include "Import.h"
#include "Reni.h"

using namespace _Reni;
using namespace Reni;

void _Reni::RunAll()
{
    Compiler c = __FILE__;
    auto s = c.source;
    auto t = s.Text;
    assertx(t == "", t);
}