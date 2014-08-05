#include "Import.h"
#include "Parser.h"
#include "Reni.h"
#include "HWLib.h"
#include "../HWLib/RefCountContainer.instance.h"
#include "../HWAnalyse/TestFixture.h"

int main()
{       
    DumpableObject::EnableSetDumpString = true;
    DumpableObject::EnableSetDumpStringAsync = true;
    test_item(_Reni::Negative).isLowPriority = true;
    test_item(_Reni::PrimitiveRecursiveFunctionByteWithDump).WatchedRun();
    HWAnalyse::TestFixture::RunAll();
    return 0;
}
