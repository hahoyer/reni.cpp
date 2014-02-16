#include "Import.h"
#include "Main.h"

#include "HWLib.h"
#include "Reni.h"

int main()
{
    Test::RunAll();
    return 0;
}

void Test::RunAll()
{
    auto xx = new int[200];
    _HWLib::RunAll();
    _Reni::RunAll();
}

void Test::RunSpecial()
{
    _HWLib::RunSpecial();
}

