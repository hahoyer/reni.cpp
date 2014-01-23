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
    _HWLib::RunAll();
    _Reni::RunAll();
}

