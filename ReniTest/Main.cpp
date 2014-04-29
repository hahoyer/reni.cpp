#include "Import.h"
#include "Parser.h"
#include "Reni.h"
#include "HWLib.h"
#include "../HWLib/RefCountContainer.instance.h"
#include "../HWAnalyse/TestFixture.h"

int main()
{
    DumpableObject::EnableSetDumpString = true;
    DumpableObject::EnableSetDumpStringAsync = false;
    test_item(_Reni::Negative).isLowPriority = true;
    test_item(_Reni::RecursiveFunction).WatchedRun();
    HWAnalyse::TestFixture::RunAll();
    return 0;
}
