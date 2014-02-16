#include "Import.h"
#include "Scanner.h"

#include "Reni.h"
#include "SimpleTokenFactory.h"

static bool Trace = true;

using namespace HWLang;
using namespace _Scanner;
using namespace _Compiler;


void _Scanner::SimpleAndDetailed()
{
    auto file = File("1.reni");
    file.Data = " asd f";
    _console_ WriteLine(String::FilePosition(file.FullName, 1, 3, ""));
    auto s = Source::FromFile(file.Name);
    auto sc = _Compiler::ScannerInstance(s);

    auto ss = sc.Step();
    auto pp = ss.Part;
    String t = pp;
    assert(t == "asd");
}

void Test(String text, Array<String> results)
{
    auto s = Source::FromText(text);
    auto sc = _Compiler::ScannerInstance(s);
    auto ss = sc.ToArray();
    auto i = 0;
    for (; i < results.Count && i < ss.Count; i++)
        assertx(results[i] == ss[i].Part, vardump(i) + vardump(ss[i].Part) + vardump(results[i]));
    for (; i < results.Count; i++)
        assert_failx(vardump(i) + vardump(results[i]));
    for (; i < ss.Count; i++)
        assert_failx(vardump(i) + vardump(ss[i].Part));
}

void _Scanner::Simple()
{
    Test(" asd \"cc\" 1234 ",
    {
        "asd",
        "\"cc\"",
        "1234",
        ""
    });
}

void _Scanner::Text()
{
    Test(" \"a\"\" b\" '\" ' ",
    {
        "\"a\"\" b\"",
        "'\" '",
        ""
    });
}

void _Scanner::Comment()
{
    Test(R"(
12## line comment
asdf
abc#( comment 
)#de
#(- comment #) still comment -)#


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

void _Scanner::LineCommentError()
{
    Test(R"(
12## line comment
asdf
## invalid line comment)"
,
{
        "12",
        "asdf",
        "\n## invalid line comment",
        ""
    });
};

void _Scanner::CommentError()
{
    Test(R"(
12## line comment
asdf
#( invalid comment
)"
,
{
        "12",
        "asdf",
        "\n#( invalid comment\n",
        ""
    });
};

void _Scanner::Pattern()
{
    auto s = Source::FromText("asdf") + 0;

    auto p = End.Find + HWLang::Error(String("x"));
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