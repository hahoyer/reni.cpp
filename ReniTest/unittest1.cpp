#include "Import.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ReniTest
{
    TEST_CLASS(UnitTest1)
    {
    public:

        TEST_METHOD(TestMethod1)
        {
            auto f = [](int i){return i; };
            auto x = Array<int>(3, f);

            _console_ Write("Hallo");
        }

    };
}