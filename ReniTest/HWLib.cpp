#include "Import.h"
#include "HWLib.h"

#include "../HWLang/Source.h"
#include "../HWLib/Process.h"
#include "../HWLib/System.h"
#include "../HWAnalyse/TestMacros.h"
#include "../HWAnalyse/TestFixture.h"
#include "../HWLib/StackTrace.h"
#include "../HWLib/FunctionCache.h"

using namespace HWLang;
using namespace HWLib;

static bool Trace = true;

namespace _HWLib{

    HW_TM_TEST(FileTest){
        auto s = Source::FromFile(__FILE__);
        auto t = s.Text;
        HW_ASSERT_IS(t.substr(0, 8), == , "#include");
    }

    HW_TM_TEST(StackTrace){
        auto t = HWLib::Thread::FormatStackTraceOfCurrentThread(0);
        HW_D_LOG(t);
        //HW_BREAKPOINT;
    }
}

namespace _Process
{
    HW_TM_TEST(Simple)
    {
        auto p = Process("echo example");
        auto t = p.data;
        HW_ASSERT_IS(t, == , "example\r\n");
    };

    HW_TM_TEST(StartProgram, Simple)
    {
        auto path = System::EnvironmentVariable("VS120COMNTOOLS");
        HW_ASSERT(path|Contains(" "), HW_D_VALUE(path));
        auto name = "..\\ide\\vb7to8.exe";
        auto p = Process("\"" + path + "\\" + name + "\"");
        auto d = p.data;
        auto e = p.errorData;
        HW_ASSERT_IS(e, == , "");
    };

    HW_TM_TEST(Double, Simple)
    {
        auto p = Process("time");
        auto t0 = p.data;
        System::Sleep(1000);
        auto t1 = p.data;
        HW_ASSERT_IS(t0, == , t1);
    };

    HW_TM_TEST(Double2, Simple)
    {
        auto p = Process("time");
        auto t0 = p.data;
        System::Sleep(1000);
        p.Execute();
        auto t1 = p.data;
        HW_ASSERT_IS(t0, != , t1);
    };

    HW_TM_TEST(Error, Simple)
    {
        auto p = Process("%");
        auto d = p.data;
        auto e = p.errorData;
        HW_ASSERT_IS(d, == , "");
        HW_ASSERT_IS(e, != , "");
    };

}

namespace _String
{
    HW_TM_TEST(WriteHallo)
    {
        HW_CONSOLE.WriteLine("Hallo");
    }

    HW_TM_TEST(Find)
    {
        string a = "Hallo";
        auto b = a|HWLib::Find("a");
        HW_ASSERT_IS(b.Value, == , 1);
        auto c = a|HWLib::Find("c");
        HW_ASSERT_(!c.IsValid);
    }

    HW_TM_TEST(Part)
    {
        string a = "Hallo";
        string b = a.substr(1);
        HW_ASSERT_IS(b, == , "al");
    }

    HW_TM_TEST(Plus)
    {
        string a = "Hallo";
        string b = a + a;
        HW_ASSERT_IS(b, == , "HalloHallo");
    }

    HW_TM_TEST(Split)
    {
        string a = "A B C";
        auto b = (a|HWLib::Split(" "))->ToArray;
        HW_ASSERT_IS(b.Count, == , 3);
        HW_ASSERT_IS(b[0], == , "A");

    }

    HW_TM_TEST(Stringify)
    {
        string a = "A B C";
        auto split = (a|HWLib::Split(" "))->ToArray;
        HW_ASSERT_IS(split.Count, == , 3);

        auto b = split.Stringify(".");
        HW_ASSERT_IS(b, == , "A.B.C");

    }

    HW_TM_TEST(Replace)
    {
        string a = "A B C";
        auto b = a|HWLib::Replace(" ", ".");
        HW_ASSERT_IS(b.size(), == , a.size());
        HW_ASSERT_IS(b, == , "A.B.C");

    }
    HW_TM_TEST(Replace1) {
        string a = "DumpPrint($(arg))";
        auto b = a|HWLib::Replace("$(arg)", "3");
        HW_ASSERT_IS(b, == , "DumpPrint(3)");

    }

}

namespace _Ref
{
    HW_TM_TEST(WriteHallo)
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

namespace _Array
{
    HW_TM_TEST(WriteHallo)
    {
        auto c = Numbers(3)->ToArray;
        HW_ASSERT_(c.Count == 3);
        HW_ASSERT_(c[0] == 0);
        HW_ASSERT_(c[1] == 1);
        HW_ASSERT_(c[2] == 2);
    }

}

namespace _Enumerable
{
    HW_TM_TEST(FromInt0)
    {
        Array<Array<int>> c;
        auto cc = c.ConvertMany<int>()->ToArray;
        HW_ASSERT_(cc.Count == 0);
    }

    HW_TM_TEST(FromInt1)
    {
        Array<Array<int>> c = {{12}};
        auto cc = c.ConvertMany<int>()->ToArray;
        HW_ASSERT_(cc.Count == 1);
        HW_ASSERT_(cc[0] == 12);
    }

    HW_TM_TEST(FromInt1_1)
    {
        Array<Array<int>> c = {{12}, {13}};
        auto cc = c.ConvertMany<int>()->ToArray;
        HW_ASSERT_(cc.Count == 2);
        HW_ASSERT_(cc[0] == 12);
        HW_ASSERT_(cc[1] == 13);
    }

    HW_TM_TEST(FromInt1_2)
    {
        Array<Array<int>> c = {{12}, {13, 14}};
        auto cc = c.ConvertMany<int>()->ToArray;
        HW_ASSERT_(cc.Count == 3);
        HW_ASSERT_(cc[0] == 12);
        HW_ASSERT_(cc[1] == 13);
        HW_ASSERT_(cc[2] == 14);
    }

    HW_TM_TEST(FromInt0_1_1)
    {
        Array<Array<int>> c = {{}, {12}, {13}};
        auto cc = c.ConvertMany<int>()->ToArray;
        HW_ASSERT_(cc.Count == 2);
        HW_ASSERT_(cc[0] == 12);
        HW_ASSERT_(cc[1] == 13);
    }

    HW_TM_TEST(FromString)
    {
        Array<Array<string>> c = {{"asdf"}};
        auto cc = c.ConvertMany<string>()->ToArray;
        HW_ASSERT_(cc.Count == 1);
        HW_ASSERT_(cc[0] == "asdf");
    }

}

namespace _ValueCache
{
    HW_TM_TEST(Simple)
    {
        ValueCache<int> c([](){return 12; });
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
    HW_TM_TEST(Context)
    {
        _value = 12;
        ValueCache<int>c = ([&]{return _value; });
        c.IsValid = true;
        HW_ASSERT_IS(c.Value, == , 12);
        _value = 13;
        HW_ASSERT_IS(c.Value, == , 12);
        c.IsValid = true;
        HW_ASSERT_IS(c.Value, == , 12);
        c.IsValid = false;
        c.IsValid = true;
        HW_ASSERT_IS(c.Value, == , 13);
        _value = 14;
        c.IsValid = false;
        HW_ASSERT_IS(c.Value, == , 14);
    }

    class Container{
        int const valueItSelf;
    public:
        Container(int value)
            : valueItSelf(value)
            , fixValueCache([=]{return valueItSelf; })
            , functionValueCache([=]{return GetValue(value); })
        {}

        int const GetValue(int value)const{ return value + valueItSelf; }

        ValueCache<int> fixValueCache;
        ValueCache<int> functionValueCache;
    };

    HW_TM_TEST(Member){
        Container c(17);

        HW_ASSERT_(!c.fixValueCache.IsValid);
        HW_ASSERT_IS(c.fixValueCache.Value, == , 17);
        HW_ASSERT_(c.fixValueCache.IsValid);
        HW_ASSERT_IS(c.functionValueCache.Value, == , 34);

    }

}


namespace _FunctionCache{
    HW_TM_TEST(Simple){
        FunctionCache<int, int> c([](int x){return 12+x; });
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

    HW_TM_TEST(Context, Simple){
        int value = 12;
        FunctionCache<int, int> c([&](int x){return value + x; });
        HW_ASSERT_(c(1) == 13);
        value = 1;
        HW_ASSERT_(c(1) == 13);
        HW_ASSERT_(c(2) == 3);
        c.IsValid(1, false);
        HW_ASSERT_(c(1) == 2);
    }

    HW_TM_TEST(Multiple, Simple){
        int value = 1;
        FunctionCache<size_t, string, size_t> c([&](string x, size_t y){return value + x.size()+ y;});
        HW_ASSERT_(c("ss",1) == 4);
        value = 2;
        HW_ASSERT_(c("ss", 1) == 4);
    }

}