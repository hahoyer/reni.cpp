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
    test_(FileTest){
        auto s = Source::FromFile(__FILE__);
        auto t = s.Text;
        a_is(t.Part(0, 8), == , "#include");
    }

    test_(StackTrace){
        auto t = HWLib::Thread::FormatStackTraceOfCurrentThread(0);
        dd(t);
        //b_;
    }
}

namespace _Process
{
    test_(Simple)
    {
        auto p = Process("echo example");
        auto t = p.data;
        a_is(t, == , "example\r\n");
    };

    test(StartProgram, Simple)
    {
        auto path = System::EnvironmentVariable("VS120COMNTOOLS");
        a_if(path.Contains(" "), nd(path));
        auto name = "..\\ide\\vb7to8.exe";
        auto p = Process("\"" + path + "\\" + name + "\"");
        auto d = p.data;
        auto e = p.errorData;
        a_is(e, == , "");
    };

    test(Double, Simple)
    {
        auto p = Process("time");
        auto t0 = p.data;
        System::Sleep(1000);
        auto t1 = p.data;
        a_is(t0, == , t1);
    };

    test(Double2, Simple)
    {
        auto p = Process("time");
        auto t0 = p.data;
        System::Sleep(1000);
        p.Execute();
        auto t1 = p.data;
        a_is(t0, != , t1);
    };

    test(Error, Simple)
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
    test_(WriteHallo)
    {
        c_.WriteLine("Hallo");
    }

    test_(Find)
    {
        String a = "Hallo";
        auto b = a.Find("a");
        a_is(b.Value, == , 1);
        auto c = a.Find("c");
        a_if_(!c.IsValid);
    }

    test_(Part)
    {
        String a = "Hallo";
        String b = a.Part(1, 2);
        a_is(b, == , "al");
    }

    test_(Plus)
    {
        String a = "Hallo";
        String b = a + a;
        a_is(b, == , "HalloHallo");
    }

    test_(Split)
    {
        String a = "A B C";
        auto b = a.Split(" ")->ToArray;
        a_is(b.Count, == , 3);
        a_is(b[0], == , "A");

    }

    test_(Stringify)
    {
        String a = "A B C";
        auto split = a.Split(" ")->ToArray;
        a_is(split.Count, == , 3);

        auto b = split.Stringify(".");
        a_is(b, == , "A.B.C");

    }

    test_(Replace)
    {
        String a = "A B C";
        auto b = a.Replace(" ", ".");
        a_is(b.Count, == , a.Count);
        a_is(b, == , "A.B.C");

    }
    test_(Replace1) {
        String a = "DumpPrint($(arg))";
        auto b = a.Replace("$(arg)", "3");
        a_is(b, == , "DumpPrint(3)");

    }

}

namespace _Ref
{
    test_(WriteHallo)
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
    test_(WriteHallo)
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
    test_(FromInt0)
    {
        Array<Array<int>> c;
        auto cc = c.ConvertMany<int>()->ToArray;
        a_if_(cc.Count == 0);
    }

    test_(FromInt1)
    {
        Array<Array<int>> c = {{12}};
        auto cc = c.ConvertMany<int>()->ToArray;
        a_if_(cc.Count == 1);
        a_if_(cc[0] == 12);
    }

    test_(FromInt1_1)
    {
        Array<Array<int>> c = {{12}, {13}};
        auto cc = c.ConvertMany<int>()->ToArray;
        a_if_(cc.Count == 2);
        a_if_(cc[0] == 12);
        a_if_(cc[1] == 13);
    }

    test_(FromInt1_2)
    {
        Array<Array<int>> c = {{12}, {13, 14}};
        auto cc = c.ConvertMany<int>()->ToArray;
        a_if_(cc.Count == 3);
        a_if_(cc[0] == 12);
        a_if_(cc[1] == 13);
        a_if_(cc[2] == 14);
    }

    test_(FromInt0_1_1)
    {
        Array<Array<int>> c = {{}, {12}, {13}};
        auto cc = c.ConvertMany<int>()->ToArray;
        a_if_(cc.Count == 2);
        a_if_(cc[0] == 12);
        a_if_(cc[1] == 13);
    }

    test_(FromString)
    {
        Array<Array<String>> c = {{"asdf"}};
        auto cc = c.ConvertMany<String>()->ToArray;
        a_if_(cc.Count == 1);
        a_if_(cc[0] == "asdf");
    }

}

namespace _ValueCache
{
    test_(Simple)
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
    test_(Context)
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

    test_(Member){
        Container c(17);

        a_if_(!c.fixValueCache.IsValid);
        a_is(c.fixValueCache.Value, == , 17);
        a_if_(c.fixValueCache.IsValid);
        a_is(c.functionValueCache.Value, == , 34);

    }

}


namespace _FunctionCache{
    test_(Simple){
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

    test(Context, Simple){
        int value = 12;
        FunctionCache<int, int> c([&](int x){return value + x; });
        a_if_(c(1) == 13);
        value = 1;
        a_if_(c(1) == 13);
        a_if_(c(2) == 3);
        c.IsValid(1, false);
        a_if_(c(1) == 2);
    }

    test(Multiple, Simple){
        int value = 1;
        FunctionCache<int, String, int> c([&](String x, int y){return value + x.Count + y;});
        a_if_(c("ss",1) == 4);
        value = 2;
        a_if_(c("ss", 1) == 4);
    }

}