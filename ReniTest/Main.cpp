#include "Import.h"
#include "Compiler.h"
#include "HWLang.h"
#include "HWLib.h"
#include "Reni.h"
#include "../HWLib/RefCountContainer.instance.h"
#include "../HWAnalyse/TestFixture.h"

int main()
{
    test_item(_Reni::Negative).isLowPriority = true;
    HWAnalyse::TestFixture::RunAll();
    return 0;
}
