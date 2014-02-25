#include "Import.h"
#include "Main.h"

#include "HWLib.h"
#include "HWLang.h"
#include "Reni.h"

int main()
{
    Test::RunSpecial();
    return 0;
}

void Test::RunAll()
{
    _HWLib::RunAll();
    _HWLang::RunAll();
    _Reni::RunAll();
}

void Test::RunSpecial()
{
    _Reni::CompileSimple();
}

