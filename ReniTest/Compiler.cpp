#include "Import.h"

#include "Reni.h"
#include "CompilerTest.h"
#include <initializer_list>


static bool Trace = true;

using namespace _Reni;
using namespace _Reni::_Compiler;
using namespace Reni;

namespace _Compiler
{
    void ScannerSimpleAndDetailed()
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

    void ScannerTest(String text, Array<String> results)
    {
        CompilerTest c;
        c.Text = text;
        auto ss = c.ToArray;
        int count = results.Count;
        assertx(ss.Count == results.Count, vardump(ss.Count) + vardump(results.Count));
        for (auto i = 0; i < count; i++)
            assertx(results[i] == ss[i].Part, vardump(i) + vardump(ss[i].Part) + vardump(results[i]));
    }

    void ScannerSimple()
    {
        ScannerTest(" asd \"cc\" 1234 ",
        {
            "asd",
            "\"cc\"",
            "1234",
            ""
        });
    }

    void ScannerText()
    {
        ScannerTest(" \"a\"\" b\" '\" ' ",
        {
            "\"a\"\" b\"",
            "'\" '",
            ""
        });
    }

    void ScannerComment()
    {
        ScannerTest(R"(
12# line comment
asdf
abc#( comment 
)#de
)"
,
{
            "12",
            "asdf",
            "abc",
            "de",
            ""
        });
    };

    void ScannerError()
    {
        ScannerTest(R"(
12# line comment
asdf
# invalid line comment)"
,
{
            "12",
            "asdf",
            ""
        });
    };

    void Pattern()
    {
        auto s = Source::FromText("asdf") + 0;
        
        auto p = End.Find + Match::Error(String("x"));
        try
        {
            auto sp = p.Match(s);

        }
        catch (Exception<String> xxx)
        {
            assert(xxx.Position.IsEnd);
            assert(xxx.Error == "x");
        }


    }
    
    void RunAll()
    {
        Pattern();
        ScannerSimpleAndDetailed();
        ScannerSimple();
        ScannerText();
        ScannerComment();
    }
}