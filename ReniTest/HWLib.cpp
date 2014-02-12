#include "Import.h"
#include "HWLib.h"

using namespace ::HWLib;
using namespace _HWLib;

namespace _File
{
    void RunAll()
    {
        Compiler c = __FILE__;
        auto s = c.source;
        auto t = s->Text;
        assertx(t.Part(0, 8) == "#include", t);
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
        assert(b == 1);
        auto c = a.Find("c");
        assert(!c.IsValid);
    }

    void Part()
    {
        String a = "Hallo";
        String b = a.Part(1,2);
        assertx(b == "al", vardump(b));
    }

    void Plus()
    {
        String a = "Hallo";
        String b = a + a;
        assert(b == "HalloHallo");
    }

    void Split()
    {
        String a = "A B C";
        auto b = a.Split(" ")->ToArray;
        assertx(b.Count == 3, vardump(b.Count));
        assert(b[0] == "A");

    }

    void Stringify()
    {
        String a = "A B C";
        auto split = a.Split(" ")->ToArray;
        assertx(split.Count == 3, vardump(split.Count));

        auto b = split.Stringify(".");
        assert(b == "A.B.C");

    }

    void Replace()
    {
        String a = "A B C";
        auto b = a.Replace(" ", ".");
        assertx(b.Count == a.Count, vardump(b.Count));
        assert(b == "A.B.C");

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

        assert(!c.IsValid);
        c = new int(1);
        assert(c.IsValid);
        c = OptRef<int>();
        assert(!c.IsValid);

        c = new int(12);
        assert(*c == 12);
        assert(c.IsValid);
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
        assert(c.Count == 3);
        assert(c[0] == 0);
        assert(c[1] == 1);
        assert(c[2] == 2);
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
        assert(cc.Count == 0);
    }

    void FromInt1()
    {
        Array<Array<int>> c = { { 12 } };
        auto cc = c.ConvertMany<int>()->ToArray;
        assert(cc.Count == 1);
        assert(cc[0]== 12);
    }

    void FromInt1_1()
    {
        Array<Array<int>> c = { { 12 }, { 13 } };
        auto cc = c.ConvertMany<int>()->ToArray;
        assert(cc.Count == 2);
        assert(cc[0] == 12);
        assert(cc[1] == 13);
    }

    void FromInt1_2()
    {
        Array<Array<int>> c = { { 12 }, { 13, 14 } };
        auto cc = c.ConvertMany<int>()->ToArray;
        assert(cc.Count == 3);
        assert(cc[0] == 12);
        assert(cc[1] == 13);
        assert(cc[2] == 14);
    }

    void FromString()
    {
        Array<Array<String>> c = { { "asdf" } };
        auto cc = c.ConvertMany<String>()->ToArray;
        assert(cc.Count == 1);
        assert(cc[0] == "asdf");
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
        assert(!c.IsValid);
        c.IsValid = true;
        assert(c.IsValid);
        c.IsValid = false;
        assert(!c.IsValid);
        auto z = *c.Value;
        auto zz = *c.Value;
        assert(*c.Value == 12);
        assert(c.IsValid);
    }

    int _value;
    void Context()
    {
        _value = 12;
        ValueCache<int>c = ([&]{return _value; });
        c.IsValid = true;
        assert(*c.Value == 12);
        _value = 13;
        assert(*c.Value == 12);
        c.IsValid = true;
        assert(*c.Value == 12);
        c.IsValid = false;
        c.IsValid = true;
        assert(*c.Value == 13);
        _value = 14;
        c.IsValid = false;
        assertx(*c.Value == 14, vardump(*c.Value));
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
