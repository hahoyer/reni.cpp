#include "Import.h"
#include "Reni.h"

using namespace _Reni;
using namespace Reni;

void SimpleFile()
{
    Compiler c = __FILE__;
    auto s = c.source;
    auto t = s->Text;
    assertx(t.Part(0, 8) == "#include", t);
};


void _Reni::RunAll()
{
    SimpleFile();
    Split();
}