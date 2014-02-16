#include "Import.h"
#include "HWLib.h"

using namespace Reni;
using namespace HWLang;
using namespace _HWLib;

namespace _File
{
    void RunAll()
    {
        Compiler c = __FILE__;
        auto s = c.source;
        auto t = s->Text;
        a_is(t.Part(0, 8), == ,"#include");
    };
}

namespace _String
{
    void WriteHallo()
    {
        _console_ WriteLine("Hallo");
    }

    void Find()
    {
        String a = "Hallo";
        auto b = a.Find("a");
        a_is(b, ==, 1);
        auto c = a.Find("c");
        a_if_(!c.IsValid);
    }

    void Part()
    {
        String a = "Hallo";
        String b = a.Part(1,2);
        a_is(b, ==, "al");
    }

    void Plus()
    {
        String a = "Hallo";
        String b = a + a;
        a_is(b, ==, "HalloHallo");
    }

    void Split()
    {
        String a = "A B C";
        auto b = a.Split(" ")->ToArray;
        a_is(b.Count, ==, 3);
        a_is(b[0], ==, "A");

    }

    void Stringify()
    {
        String a = "A B C";
        auto split = a.Split(" ")->ToArray;
        a_is(split.Count, ==, 3);

        auto b = split.Stringify(".");
        a_is(b, ==, "A.B.C");

    }

    void Replace()
    {
        String a = "A B C";
        auto b = a.Replace(" ", ".");
        a_is(b.Count, ==, a.Count);
        a_is(b, ==, "A.B.C");

    }

    void RunAll()
    {
        Part();
        Find();
        Plus();
        Split();
        Stringify();
        Replace();
        WriteHallo();
    }
}

namespace _Ref
{
    void WriteHallo()
    {
        OptRef<int> c;

        a_if_(!c.IsValid);
        c = new int(1);
        a_if_(c.IsValid);
        c = OptRef<int>();
        a_if_(!c.IsValid);

        c = new int(12);
        a_if_(*c == 12);
        a_if_(c.IsValid);
    }

    void RunAll()
    {
        WriteHallo();
    }
}

namespace _Array
{
    void WriteHallo()
    {
        auto c = Array<int>(3, [](int i){return i; });
        a_if_(c.Count == 3);
        a_if_(c[0] == 0);
        a_if_(c[1] == 1);
        a_if_(c[2] == 2);
    }

    void RunAll()
    {
        WriteHallo();
    }
}

namespace _Enumerable
{
    void FromInt0()
    {
        Array<Array<int>> c ;
        auto cc = c.ConvertMany<int>()->ToArray;
        a_if_(cc.Count == 0);
    }

    void FromInt1()
    {
        Array<Array<int>> c = { { 12 } };
        auto cc = c.ConvertMany<int>()->ToArray;
        a_if_(cc.Count == 1);
        a_if_(cc[0]== 12);
    }

    void FromInt1_1()
    {
        Array<Array<int>> c = { { 12 }, { 13 } };
        auto cc = c.ConvertMany<int>()->ToArray;
        a_if_(cc.Count == 2);
        a_if_(cc[0] == 12);
        a_if_(cc[1] == 13);
    }

    void FromInt1_2()
    {
        Array<Array<int>> c = { { 12 }, { 13, 14 } };
        auto cc = c.ConvertMany<int>()->ToArray;
        a_if_(cc.Count == 3);
        a_if_(cc[0] == 12);
        a_if_(cc[1] == 13);
        a_if_(cc[2] == 14);
    }

    void FromString()
    {
        Array<Array<String>> c = { { "asdf" } };
        auto cc = c.ConvertMany<String>()->ToArray;
        a_if_(cc.Count == 1);
        a_if_(cc[0] == "asdf");
    }

    void RunAll()
    {
        FromInt0();
        FromInt1();
        FromInt1_1();
        FromInt1_2();
        FromString();
    }
}

namespace _ValueCache
{
    void Simple()
    {
        ValueCache<int> c ([](){return 12; });
        a_if_(!c.IsValid);
        c.IsValid = true;
        a_if_(c.IsValid);
        c.IsValid = false;
        a_if_(!c.IsValid);
        auto z = *c.Value;
        auto zz = *c.Value;
        a_if_(*c.Value == 12);
        a_if_(c.IsValid);
    }

    int _value;
    void Context()
    {
        _value = 12;
        ValueCache<int>c = ([&]{return _value; });
        c.IsValid = true;
        a_is(*c.Value, ==, 12);
        _value = 13;
        a_is(*c.Value, == , 12);
        c.IsValid = true;
        a_is(*c.Value, == , 12);
        c.IsValid = false;
        c.IsValid = true;
        a_is(*c.Value, == , 13);
        _value = 14;
        c.IsValid = false;
        a_is(*c.Value ,==, 14);
    }

    void RunAll()
    {
        Simple();
        Context();
    }
}

void _HWLib::RunAll()
{
    _String::RunAll();
    _Array::RunAll();
    _Enumerable::RunAll();
    _Ref::RunAll();
    _ValueCache::RunAll();
    _File::RunAll();
}

void _HWLib::RunSpecial()
{
    _String::Split();
}
