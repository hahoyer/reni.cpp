#pragma once
#include "Test.h"
#include "../HWLang/Source.h"
#include "../HWLib/Process.h"
#include "../HWLib/System.h"

using namespace HWLang;
using namespace HWLib;

static bool Trace = true;

test(FileTest){
    auto s = Source::FromFile(__FILE__);
    auto t = s.Text;
    a_is(t.Part(0, 8), == , "#include");
}

namespace _Process
{
    test(Simple)
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

    test(Double)
    {
        auto p = Process("time");
        auto t0 = p.data;
        System::Sleep(1000);
        auto t1 = p.data;
        a_is(t0, == , t1);
    };

    test(Double2)
    {
        auto p = Process("time");
        auto t0 = p.data;
        System::Sleep(1000);
        p.Execute();
        auto t1 = p.data;
        a_is(t0, != , t1);
    };

    test(Error)
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
    test(WriteHallo)
    {
        _console_ WriteLine("Hallo");
    }

    test(Find)
    {
        String a = "Hallo";
        auto b = a.Find("a");
        a_is(b, == , 1);
        auto c = a.Find("c");
        a_if_(!c.IsValid);
    }

    test(Part)
    {
        String a = "Hallo";
        String b = a.Part(1, 2);
        a_is(b, == , "al");
    }

    test(Plus)
    {
        String a = "Hallo";
        String b = a + a;
        a_is(b, == , "HalloHallo");
    }

    test(Split)
    {
        String a = "A B C";
        auto b = a.Split(" ")->ToArray;
        a_is(b.Count, == , 3);
        a_is(b[0], == , "A");

    }

    test(Stringify)
    {
        String a = "A B C";
        auto split = a.Split(" ")->ToArray;
        a_is(split.Count, == , 3);

        auto b = split.Stringify(".");
        a_is(b, == , "A.B.C");

    }

    test(Replace)
    {
        String a = "A B C";
        auto b = a.Replace(" ", ".");
        a_is(b.Count, == , a.Count);
        a_is(b, == , "A.B.C");

    }
    test(Replace1) {
        String a = "DumpPrint($(arg))";
        auto b = a.Replace("$(arg)", "3");
        a_is(b, == , "DumpPrint(3)");

    }

}

namespace _Ref
{
    test(WriteHallo)
    {
        CtrlPtr<int> c;

        a_if_(c.IsEmpty);
        c = new int(1);
        a_if_(!c.IsEmpty);
        c = CtrlPtr<int>();
        a_if_(c.IsEmpty);

        c = new int(12);
        a_if_(*c == 12);
        a_if_(!c.IsEmpty);
    }
}

namespace _Array
{
    test(WriteHallo)
    {
        auto c = Array<int>(3, [](int i){return i; });
        a_if_(c.Count == 3);
        a_if_(c[0] == 0);
        a_if_(c[1] == 1);
        a_if_(c[2] == 2);
    }

}

namespace _Enumerable
{
    test(FromInt0)
    {
        Array<Array<int>> c;
        auto cc = c.ConvertMany<int>()->ToArray;
        a_if_(cc.Count == 0);
    }

    test(FromInt1)
    {
        Array<Array<int>> c = {{12}};
        auto cc = c.ConvertMany<int>()->ToArray;
        a_if_(cc.Count == 1);
        a_if_(cc[0] == 12);
    }

    test(FromInt1_1)
    {
        Array<Array<int>> c = {{12}, {13}};
        auto cc = c.ConvertMany<int>()->ToArray;
        a_if_(cc.Count == 2);
        a_if_(cc[0] == 12);
        a_if_(cc[1] == 13);
    }

    test(FromInt1_2)
    {
        Array<Array<int>> c = {{12}, {13, 14}};
        auto cc = c.ConvertMany<int>()->ToArray;
        a_if_(cc.Count == 3);
        a_if_(cc[0] == 12);
        a_if_(cc[1] == 13);
        a_if_(cc[2] == 14);
    }

    test(FromInt0_1_1)
    {
        Array<Array<int>> c = {{}, {12}, {13}};
        auto cc = c.ConvertMany<int>()->ToArray;
        a_if_(cc.Count == 2);
        a_if_(cc[0] == 12);
        a_if_(cc[1] == 13);
    }

    test(FromString)
    {
        Array<Array<String>> c = {{"asdf"}};
        auto cc = c.ConvertMany<String>()->ToArray;
        a_if_(cc.Count == 1);
        a_if_(cc[0] == "asdf");
    }

}

namespace _ValueCache
{
    test(Simple)
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
    test(Context)
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

    test(Member){
        Container c(17);

        a_if_(!c.fixValueCache.IsValid);
        a_is(c.fixValueCache.Value, == , 17);
        a_if_(c.fixValueCache.IsValid);
        a_is(c.functionValueCache.Value, == , 34);

    }

}
