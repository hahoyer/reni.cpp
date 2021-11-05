#include "Import.h"
#include "Parser.h"
#include "Reni.h"
#include "HWLib.h"
#include "../HWLib/RefCountContainer.instance.h"
#include "../HWAnalyse/TestFixture.h"

void MainEntry()
{
    DumpableObject::EnableSetDumpString = true;
    DumpableObject::EnableSetDumpStringAsync = true;
    test_item(_Reni::Negative).isLowPriority = true;
    test_item(_Reni::Function).WatchedRun();
    HWAnalysis::TestFixture::RunAll();
}


int main()
{
    MainEntry();
    return 0;
}
