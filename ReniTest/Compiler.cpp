#include "Import.h"

#include "Reni.h"

static bool Trace = true;

using namespace _Reni;
using namespace Reni;

namespace _Compiler
{
    void SimpleAndDetailed()
    {
        auto file = File("1.reni");
        file.Data = " asd f";
        _console_ WriteLine(String::FilePosition(file.FullName, 1, 3, ""));
        Compiler c = file.Name;
        auto s = c.source;
        auto tf = MainTokenFactory();
        Scanner<ReniScanner, MainTokenFactory, Token> scanner(s, tf);

        auto ss = scanner.Step();
        auto pp = ss.Part;
        String t = pp;
        assert(t == "asd");
    }

    class CompilerTest final
    {
        File _file;
        Compiler _compiler;

        CompilerTest(String name)
            : _file(name)
            , _compiler(name)
        {
        };

    public:
        CompilerTest()
            : CompilerTest("compilerTest.reni")
        {
        };

        p_mutable(String, Text){ return _file.Data; }
        p(Array<Token>, ToArray);
    };

    p_mutator_implementation(CompilerTest, String, Text){ _file.Data = value; };
    p_implementation(CompilerTest, Array<Token>, ToArray)
    {
        return Compiler::Scanner(*_compiler.scanner).ToArray();
    };

    void Simple()
    {
        CompilerTest c;
        c.Text = " asd f";
        auto ss = c.ToArray;
        assertx(ss.Count == 3, vardump(ss.Count));
        assert(String(ss[0].Part) == "asd");
        assert(String(ss[1].Part) == "f");
        assert(String(ss[0].Part) == "");
    }

    void RunAll()
    {
        SimpleAndDetailed();
        Simple();
    }
}