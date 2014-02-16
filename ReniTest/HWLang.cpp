#include "Import.h"
#include "HWLang.h"

#include "Reni.h"
#include "SimpleTokenFactory.h"

static bool Trace = true;

using namespace HWLang;
using namespace _HWLang;


void _HWLang::SimpleAndDetailed()
{
    auto file = File("1.reni");
    file.Data = " asd f";
    _console_ WriteLine(String::FilePosition(file.FullName, 1, 3, ""));
    auto s = Source::FromFile(file.Name);
    auto sc = ScannerInstance(s);

    TokenClass const& start = sc.Step().Class;
    a_is(&start, == , &ScannerInstance::TokenFactory::Start);
    String t = sc.Step().Part;
    a_is(t, ==, "asd");
}

void Test(String text, Array<String> results)
{
    auto s = Source::FromText(text);
    auto sc = _HWLang::ScannerInstance(s);
    auto ss = sc.ToArray();
    auto i = 0;
    for (; i < results.Count && i < ss.Count; i++)
        a_if(results[i] == ss[i].Part, nd(i) + nd(ss[i].Part) + nd(results[i]));
    for (; i < results.Count; i++)
        a_fail(nd(i) + nd(results[i]));
    for (; i < ss.Count; i++)
        a_fail(nd(i) + nd(ss[i].Part));
}

void _HWLang::Simple()
{
    Test(" asd \"cc\" 1234 ",
    {
        "",
        "asd",
        "\"cc\"",
        "1234",
        ""
    });
}

void _HWLang::Text()
{
    Test(" \"a_if\"\" b\" '\" ' ",
    {
        "",
        "\"a_if\"\" b\"",
        "'\" '",
        ""
    });
}

void _HWLang::Comment()
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
        "",
        "12",
        "asdf",
        "abc",
        "de",
        ""
    });
};

void _HWLang::LineCommentError()
{
    Test(R"(
12## line comment
asdf
## invalid line comment)"
,
{
        "",
        "12",
        "asdf",
        "\n## invalid line comment",
        ""
    });
};

void _HWLang::CommentError()
{
    Test(R"(
12## line comment
asdf
#( invalid comment
)"
,
{
        "",
        "12",
        "asdf",
        "\n#( invalid comment\n",
        ""
    });
};

void _HWLang::Pattern()
{
    auto s = Source::FromText("asdf") + 0;

    auto p = End.Find + HWLang::Error(String("x"));
    try
    {
        auto sp = p.Match(s);
    }
    catch (Exception<String> xxx)
    {
        a_if_(xxx.Position.IsEnd);
        a_if_(xxx.Error == "x");
    }
}