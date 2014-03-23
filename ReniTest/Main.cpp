#include "Import.h"
#include "Test.h"
#include "Compiler.h"
#include "HWLang.h"
#include "HWLib.h"
#include "Reni.h"
#include "../HWLib/RefCountContainer.instance.h"

int main()
{
    test_item(FileTest).isLowPriority = true;
    HWTest::TestFixture::RunAll();
    return 0;
}
