#include "Import.h"
#include "Parser.h"
#include "Reni.h"
#include "../HWLib/RefCountContainer.instance.h"
#include "../HWAnalyse/TestFixture.h"

int main()
{
    test_item(_Reni::Negative).isLowPriority = true;
    test_item(_Reni::RecursiveFunction).WatchedRun();
    HWAnalyse::TestFixture::RunAll();
    return 0;
}
