#include "Import.h"
#include "Reni.h"

#include "Scanner.h"

using namespace _Reni;
using namespace Reni;

namespace _Compiler
{
    void RunAll();
}

void _Reni::RunAll()
{
    _Scanner::RunAll();
    _Compiler::RunAll();

}