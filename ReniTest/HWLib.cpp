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
    HW_TM_TEST_(FileTest){
        auto s = Source::FromFile(__FILE__);
        auto t = s.Text;
        a_is(t.substr(0, 8), == , "#include");
    }

    HW_TM_TEST_(StackTrace){
        auto t = HWLib::Thread::FormatStackTraceOfCurrentThread(0);
        dd(t);
        //b_;
    }
}

namespace _Process
{
    HW_TM_TEST_(Simple)
    {
        auto p = Process("echo example");
        auto t = p.data;
        a_is(t, == , "example\r\n");
    };

    HW_TM_TEST(StartProgram, Simple)
    {
        auto path = System::EnvironmentVariable("VS120COMNTOOLS");
        a_if(path|Contains(" "), nd(path));
        auto name = "..\\ide\\vb7to8.exe";
        auto p = Process("\"" + path + "\\" + name + "\"");
        auto d = p.data;
        auto e = p.errorData;
        a_is(e, == , "");
    };

    HW_TM_TEST(Double, Simple)
    {
        auto p = Process("time");
        auto t0 = p.data;
        System::Sleep(1000);
        auto t1 = p.data;
        a_is(t0, == , t1);
    };

    HW_TM_TEST(Double2, Simple)
    {
        auto p = Process("time");
        auto t0 = p.data;
        System::Sleep(1000);
        p.Execute();
        auto t1 = p.data;
        a_is(t0, != , t1);
    };

    HW_TM_TEST(Error, Simple)
    {
        auto p = Process("%");
        auto d = p.data;
        auto e = p.errorData;
        a_is(d, == , "");
        a_is(e, != , "");
    };

}

namespace _String
{
    HW_TM_TEST_(WriteHallo)
    {
        c_.WriteLine("Hallo");
    }

    HW_TM_TEST_(Find)
    {
        string a = "Hallo";
        auto b = a|HWLib::Find("a");
        a_is(b.Value, == , 1);
        auto c = a|HWLib::Find("c");
        a_if_(!c.IsValid);
    }

    HW_TM_TEST_(Part)
    {
        string a = "Hallo";
        string b = a.substr(1);
        a_is(b, == , "al");
    }

    HW_TM_TEST_(Plus)
    {
        string a = "Hallo";
        string b = a + a;
        a_is(b, == , "HalloHallo");
    }

    HW_TM_TEST_(Split)
    {
        string a = "A B C";
        auto b = (a|HWLib::Split(" "))->ToArray;
        a_is(b.Count, == , 3);
        a_is(b[0], == , "A");

    }

    HW_TM_TEST_(Stringify)
    {
        string a = "A B C";
        auto split = (a|HWLib::Split(" "))->ToArray;
        a_is(split.Count, == , 3);

        auto b = split.Stringify(".");
        a_is(b, == , "A.B.C");

    }

    HW_TM_TEST_(Replace)
    {
        string a = "A B C";
        auto b = a|HWLib::Replace(" ", ".");
        a_is(b.size(), == , a.size());
        a_is(b, == , "A.B.C");

    }
    HW_TM_TEST_(Replace1) {
        string a = "DumpPrint($(arg))";
        auto b = a|HWLib::Replace("$(arg)", "3");
        a_is(b, == , "DumpPrint(3)");

    }

}

namespace _Ref
{
    HW_TM_TEST_(WriteHallo)
    {
        Optional<CtrlRef<int>> c;

        a_if_(c.IsEmpty);
        c = CtrlRef<int>(new int(1));
        a_if_(!c.IsEmpty);
        c = {};
        a_if_(c.IsEmpty);

        c = CtrlRef<int>(new int(12));
        a_if_(*c.Value == 12);
        a_if_(!c.IsEmpty);
    }
}

namespace _Array
{
    HW_TM_TEST_(WriteHallo)
    {
        auto c = Numbers(3)->ToArray;
        a_if_(c.Count == 3);
        a_if_(c[0] == 0);
        a_if_(c[1] == 1);
        a_if_(c[2] == 2);
    }

}

namespace _Enumerable
{
    HW_TM_TEST_(FromInt0)
    {
        Array<Array<int>> c;
        auto cc = c.ConvertMany<int>()->ToArray;
        a_if_(cc.Count == 0);
    }

    HW_TM_TEST_(FromInt1)
    {
        Array<Array<int>> c = {{12}};
        auto cc = c.ConvertMany<int>()->ToArray;
        a_if_(cc.Count == 1);
        a_if_(cc[0] == 12);
    }

    HW_TM_TEST_(FromInt1_1)
    {
        Array<Array<int>> c = {{12}, {13}};
        auto cc = c.ConvertMany<int>()->ToArray;
        a_if_(cc.Count == 2);
        a_if_(cc[0] == 12);
        a_if_(cc[1] == 13);
    }

    HW_TM_TEST_(FromInt1_2)
    {
        Array<Array<int>> c = {{12}, {13, 14}};
        auto cc = c.ConvertMany<int>()->ToArray;
        a_if_(cc.Count == 3);
        a_if_(cc[0] == 12);
        a_if_(cc[1] == 13);
        a_if_(cc[2] == 14);
    }

    HW_TM_TEST_(FromInt0_1_1)
    {
        Array<Array<int>> c = {{}, {12}, {13}};
        auto cc = c.ConvertMany<int>()->ToArray;
        a_if_(cc.Count == 2);
        a_if_(cc[0] == 12);
        a_if_(cc[1] == 13);
    }

    HW_TM_TEST_(FromString)
    {
        Array<Array<string>> c = {{"asdf"}};
        auto cc = c.ConvertMany<string>()->ToArray;
        a_if_(cc.Count == 1);
        a_if_(cc[0] == "asdf");
    }

}

namespace _ValueCache
{
    HW_TM_TEST_(Simple)
    {
        ValueCache<int> c([](){return 12; });
        a_if_(!c.IsValid);
        c.IsValid = true;
        a_if_(c.IsValid);
        c.IsValid = false;
        a_if_(!c.IsValid);
        auto z = c.Value;
        auto zz = c.Value;
        a_if_(c.Value == 12);
        a_if_(c.IsValid);
    }

    int _value;
    HW_TM_TEST_(Context)
    {
        _value = 12;
        ValueCache<int>c = ([&]{return _value; });
        c.IsValid = true;
        a_is(c.Value, == , 12);
        _value = 13;
        a_is(c.Value, == , 12);
        c.IsValid = true;
        a_is(c.Value, == , 12);
        c.IsValid = false;
        c.IsValid = true;
        a_is(c.Value, == , 13);
        _value = 14;
        c.IsValid = false;
        a_is(c.Value, == , 14);
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

    HW_TM_TEST_(Member){
        Container c(17);

        a_if_(!c.fixValueCache.IsValid);
        a_is(c.fixValueCache.Value, == , 17);
        a_if_(c.fixValueCache.IsValid);
        a_is(c.functionValueCache.Value, == , 34);

    }

}


namespace _FunctionCache{
    HW_TM_TEST_(Simple){
        FunctionCache<int, int> c([](int x){return 12+x; });
        a_if_(!c.IsValid(1));
        c.IsValid(1, true);
        a_if_(c.IsValid(1));
        c.IsValid(1, false);
        a_if_(!c.IsValid(1));
        auto z = c(1);
        auto zz = c(2);
        a_if_(c(1) == 13);
        a_if_(c.IsValid(2));
    }

    HW_TM_TEST(Context, Simple){
        int value = 12;
        FunctionCache<int, int> c([&](int x){return value + x; });
        a_if_(c(1) == 13);
        value = 1;
        a_if_(c(1) == 13);
        a_if_(c(2) == 3);
        c.IsValid(1, false);
        a_if_(c(1) == 2);
    }

    HW_TM_TEST(Multiple, Simple){
        int value = 1;
        FunctionCache<size_t, string, size_t> c([&](string x, size_t y){return value + x.size()+ y;});
        a_if_(c("ss",1) == 4);
        value = 2;
        a_if_(c("ss", 1) == 4);
    }

}