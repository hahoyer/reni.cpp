#include "Import.h"
#include "CppUnitTest.h"

using namespace ::HWLib;

namespace Test
{
    namespace _HWLib
    {
        namespace _String
        {
            void WriteHallo()
            {
                _console_ WriteLine("Hallo");
            }

            void Plus()
            {
                String a = "Hallo";
                String b = a + a;
                assert(b == "HalloHallo");
            }

            void RunAll()
            {
                Plus();
                WriteHallo();
            }
        }

        namespace _Ref
        {
            void WriteHallo()
            {
                OptRef<int> c;

                assert(!c.IsValid);
                c = 1;
                assert(c.IsValid);
                c = OptRef<int>();
                assert(!c.IsValid);

                c = 12;
                assert(*c == 12);
                assert(c.IsValid);
            }

            void RunAll()
            {
                WriteHallo();
            }
        }

        namespace _ValueCache
        {
            void Simple()
            {
                auto c = ValueCache<int>([]{return 12; });

                assert(!c.IsValid);
                c.IsValid = true;
                assert(c.IsValid);
                c.IsValid = false;
                assert(!c.IsValid);
                auto z = *c;
                assert(*c == 12);
                assert(c.IsValid);
            }

            int _value;
            void Context()
            {
                _value = 12;
                auto c = ValueCache<int>([&]{return _value; });
                c.IsValid = true;
                assert(*c == 12);
                _value = 13;
                assert(*c == 12);
                c.IsValid = true;
                assert(*c == 12);
                c.IsValid = false;
                c.IsValid = true;
                assert(*c == 13);
                _value = 14;
                c.IsValid = false;
                assert(*c == 14);
                assertx(*c == 15, vardump(*c));
            }

            void RunAll()
            {
                Simple();
                Context();
            }
        }

        void RunAll()
        {
            _Ref::RunAll();
            _String::RunAll();
            _ValueCache::RunAll();
        }
    }

    void RunAll()
    {
        _HWLib::RunAll();
    }
}

int main()
{
    Test::RunAll();
    return 0;
}
