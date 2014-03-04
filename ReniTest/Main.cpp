#include "Import.h"
#include "Main.h"

#include "HWLib.h"
#include "HWLang.h"
#include "Reni.h"

int main()
{
    ReniTest::RunAll();
    return 0;
}

void ReniTest::RunAll()
{
    _HWLib::RunAll();
    _HWLang::RunAll();
    Common::RunAll();
}

void ReniTest::RunSpecial()
{
    ReniTest::Common::CompileSimple();
}

