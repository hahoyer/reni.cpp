#include "Import.h"
#include "HWLib.h"

#include "../HWAnalyse/TestFixture.h"
#include "../HWAnalyse/TestMacros.h"

#include "../HWLang/Source.h"

#include "../HWLib/FunctionCache.h"
#include "../HWLib/Process.h"
#include "../HWLib/StackTrace.h"
#include "../HWLib/System.h"

using namespace HWLang;
using namespace HWLib;

static bool Trace = true;

namespace TestHWLib
{
#pragma message(STRING(__cplusplus))

  HW_TM_IMPLEMENTATION(FileTest)
  {
    const auto s = Source::FromFile(__FILE__);
    const auto t = s.Text;
    HW_ASSERT_IS(t.substr(0, 8), ==, "#include");
  }

  HW_TM_IMPLEMENTATION(StackTrace)
  {
    const auto t = Thread::FormatStackTraceOfCurrentThread(0);
    HW_D_LOG(t);
    //HW_BREAKPOINT;
  }
}

namespace TestProcess
{
  HW_TM_IMPLEMENTATION(Simple)
  {
    const auto p = Process("echo example");
    const auto t = p.data;
    HW_ASSERT_IS(t, ==, "example\r\n");
  };

  HW_TM_IMPLEMENTATION(StartProgram, Simple)
  {
    const auto path = System::EnvironmentVariable("VS120COMNTOOLS");
    HW_ASSERT(path|Contains(" "), HW_D_VALUE(path));
    const auto name = "..\\ide\\vb7to8.exe";
    const auto p = Process("\"" + path + "\\" + name + "\"");
    auto d = p.data;
    const auto e = p.errorData;
    HW_ASSERT_IS(e, ==, "");
  };

  HW_TM_IMPLEMENTATION(Double, Simple)
  {
    const auto p = Process("time");
    const auto t0 = p.data;
    System::Sleep(1000);
    const auto t1 = p.data;
    HW_ASSERT_IS(t0, ==, t1);
  };

  HW_TM_IMPLEMENTATION(Double2, Simple)
  {
    auto p = Process("time");
    const auto t0 = p.data;
    System::Sleep(1000);
    p.Execute();
    const auto t1 = p.data;
    HW_ASSERT_IS(t0, !=, t1);
  };

  HW_TM_IMPLEMENTATION(Error, Simple)
  {
    const auto p = Process("%");
    const auto d = p.data;
    const auto e = p.errorData;
    HW_ASSERT_IS(d, ==, "");
    HW_ASSERT_IS(e, !=, "");
  };
}

namespace TestString
{
  HW_TM_IMPLEMENTATION(WriteHallo)
  {
    HW_CONSOLE.WriteLine("Hallo");
  }

  HW_TM_IMPLEMENTATION(Find)
  {
    const string a = "Hallo";
    const auto b = a | HWLib::Find("a");
    HW_ASSERT_IS(b.Value, ==, 1);
    const auto c = a | HWLib::Find("c");
    HW_ASSERT_(!c.IsValid);
  }

  HW_TM_IMPLEMENTATION(Part)
  {
    const string a = "Hallo";
    const auto b = a.substr(1);
    HW_ASSERT_IS(b, ==, "al");
  }

  HW_TM_IMPLEMENTATION(Plus)
  {
    const string a = "Hallo";
    const auto b = a + a;
    HW_ASSERT_IS(b, ==, "HalloHallo");
  }

  HW_TM_IMPLEMENTATION(Split)
  {
    const string a = "A B C";
    auto b = (a | HWLib::Split(" "))->ToArray;
    HW_ASSERT_IS(b.Count, ==, 3);
    HW_ASSERT_IS(b[0], ==, "A");
  }

  HW_TM_IMPLEMENTATION(Stringify)
  {
    const string a = "A B C";
    const auto split = (a | HWLib::Split(" "))->ToArray;
    HW_ASSERT_IS(split.Count, ==, 3);

    const auto b = split.Stringify(".");
    HW_ASSERT_IS(b, ==, "A.B.C");
  }

  HW_TM_IMPLEMENTATION(Replace)
  {
    const string a = "A B C";
    const auto b = a | HWLib::Replace(" ", ".");
    HW_ASSERT_IS(b.size(), ==, a.size());
    HW_ASSERT_IS(b, ==, "A.B.C");
  }

  HW_TM_IMPLEMENTATION(Replace1)
  {
    const string a = "DumpPrint($(arg))";
    const auto b = a | HWLib::Replace("$(arg)", "3");
    HW_ASSERT_IS(b, ==, "DumpPrint(3)");
  }
}

namespace TestRef
{
  HW_TM_IMPLEMENTATION(WriteHallo)
  {
    Optional<CtrlRef<int>> c;

    HW_ASSERT_(c.IsEmpty);
    c = CtrlRef<int>(new int(1));
    HW_ASSERT_(!c.IsEmpty);
    c = {};
    HW_ASSERT_(c.IsEmpty);

    c = CtrlRef<int>(new int(12));
    HW_ASSERT_(*c.Value == 12);
    HW_ASSERT_(!c.IsEmpty);
  }
}

namespace TestArray
{
  HW_TM_IMPLEMENTATION(WriteHallo)
  {
    auto c = Numbers(3)->ToArray;
    HW_ASSERT_(c.Count == 3);
    HW_ASSERT_(c[0] == 0);
    HW_ASSERT_(c[1] == 1);
    HW_ASSERT_(c[2] == 2);
  }
}

namespace TestEnumerable
{
  HW_TM_IMPLEMENTATION(FromInt0)
  {
    const Array<Array<int>> c;
    const auto cc = c.ConvertMany<int>()->ToArray;
    HW_ASSERT_(cc.Count == 0);
  }

  HW_TM_IMPLEMENTATION(FromInt1)
  {
    const Array<Array<int>> c = {{12}};
    auto cc = c.ConvertMany<int>()->ToArray;
    HW_ASSERT_(cc.Count == 1);
    HW_ASSERT_(cc[0] == 12);
  }

  HW_TM_IMPLEMENTATION(FromInt1_1)
  {
    const Array<Array<int>> c = {{12}, {13}};
    auto cc = c.ConvertMany<int>()->ToArray;
    HW_ASSERT_(cc.Count == 2);
    HW_ASSERT_(cc[0] == 12);
    HW_ASSERT_(cc[1] == 13);
  }

  HW_TM_IMPLEMENTATION(FromInt1_2)
  {
    const Array<Array<int>> c = {{12}, {13, 14}};
    auto cc = c.ConvertMany<int>()->ToArray;
    HW_ASSERT_(cc.Count == 3);
    HW_ASSERT_(cc[0] == 12);
    HW_ASSERT_(cc[1] == 13);
    HW_ASSERT_(cc[2] == 14);
  }

  HW_TM_IMPLEMENTATION(FromInt0_1_1)
  {
    const Array<Array<int>> c = {{}, {12}, {13}};
    auto cc = c.ConvertMany<int>()->ToArray;
    HW_ASSERT_(cc.Count == 2);
    HW_ASSERT_(cc[0] == 12);
    HW_ASSERT_(cc[1] == 13);
  }

  HW_TM_IMPLEMENTATION(FromString)
  {
    const Array<Array<string>> c = {{"asdf"}};
    auto cc = c.ConvertMany<string>()->ToArray;
    HW_ASSERT_(cc.Count == 1);
    HW_ASSERT_(cc[0] == "asdf");
  }
}

namespace TestValueCache
{
  HW_TM_IMPLEMENTATION(Simple)
  {
    ValueCache<int> c([]() { return 12; });
    HW_ASSERT_(!c.IsValid);
    c.IsValid = true;
    HW_ASSERT_(c.IsValid);
    c.IsValid = false;
    HW_ASSERT_(!c.IsValid);
    auto z = c.Value;
    auto zz = c.Value;
    HW_ASSERT_(c.Value == 12);
    HW_ASSERT_(c.IsValid);
  }

  int _value;
  HW_TM_IMPLEMENTATION(Context)
  {
    _value = 12;
    ValueCache<int> c([&] { return _value; });
    c.IsValid = true;
    HW_ASSERT_IS(c.Value, ==, 12);
    _value = 13;
    HW_ASSERT_IS(c.Value, ==, 12);
    c.IsValid = true;
    HW_ASSERT_IS(c.Value, ==, 12);
    c.IsValid = false;
    c.IsValid = true;
    HW_ASSERT_IS(c.Value, ==, 13);
    _value = 14;
    c.IsValid = false;
    HW_ASSERT_IS(c.Value, ==, 14);
  }

  class Container
  {
    const int valueItSelf;
  public:
    Container(int value)
      : valueItSelf(value)
        , fixValueCache([=] { return valueItSelf; })
        , functionValueCache([=] { return GetValue(value); })
    {}

    const int GetValue(int value) const { return value + valueItSelf; }

    ValueCache<int> fixValueCache;
    ValueCache<int> functionValueCache;
  };

  HW_TM_IMPLEMENTATION(Member)
  {
    Container c(17);

    HW_ASSERT_(!c.fixValueCache.IsValid);
    HW_ASSERT_IS(c.fixValueCache.Value, ==, 17);
    HW_ASSERT_(c.fixValueCache.IsValid);
    HW_ASSERT_IS(c.functionValueCache.Value, ==, 34);
  }
}


namespace TestFunctionCache
{
  HW_TM_IMPLEMENTATION(Simple)
  {
    const FunctionCache<int, int> c([](int x) { return 12 + x; });
    HW_ASSERT_(!c.IsValid(1));
    c.IsValid(1, true);
    HW_ASSERT_(c.IsValid(1));
    c.IsValid(1, false);
    HW_ASSERT_(!c.IsValid(1));
    auto z = c(1);
    auto zz = c(2);
    HW_ASSERT_(c(1) == 13);
    HW_ASSERT_(c.IsValid(2));
  }

  HW_TM_IMPLEMENTATION(Context, Simple)
  {
    auto value = 12;
    const FunctionCache<int, int> c([&](int x) { return value + x; });
    HW_ASSERT_(c(1) == 13);
    value = 1;
    HW_ASSERT_(c(1) == 13);
    HW_ASSERT_(c(2) == 3);
    c.IsValid(1, false);
    HW_ASSERT_(c(1) == 2);
  }

  HW_TM_IMPLEMENTATION(Multiple, Simple)
  {
    auto value = 1;
    const FunctionCache<size_t, string, size_t> c([&](string x, size_t y) { return value + x.size() + y; });
    HW_ASSERT_(c("ss",1) == 4);
    value = 2;
    HW_ASSERT_(c("ss", 1) == 4);
  }
}
