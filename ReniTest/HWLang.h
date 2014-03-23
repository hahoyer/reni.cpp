#pragma once
#include "SimpleTokenFactory.h"
#include "../HWLang/Source.h"
#include "../HWLang/Pattern.h"
#include "../HWLib/File.h"
#include "../Reni/Compiler.internal.h"

using namespace HWLang;
using namespace _HWLang;

namespace _HWLang{

    test_(SimpleAndDetailed)
    {
        using TokenFactory = SimpleTokenFactory;
        using ScannerInstance = TokenFactory::ScannerInstance;
        using TokenClass = TokenFactory::TokenClass;

        auto file = File("1.reni");
        file.Data = " asd f";
        _console_ WriteLine(String::FilePosition(file.FullName, 1, 3, ""));
        auto sc = ScannerInstance(file);

        TokenClass const& start = sc.Step().Class;
        a_is(&start, == , &TokenFactory::Start);
        String t = sc.Step().Part;
        a_is(t, == , "asd");
    }

    void Test(String const&text, Array<String> results)
    {
        using TokenFactory = SimpleTokenFactory;
        using ScannerInstance = TokenFactory::ScannerInstance;

        auto sc = ScannerInstance(text);
        auto ss = sc.ToArray;
        auto i = 0;
        for(; i < results.Count && i < ss.Count; i++)
            a_if(results[i] == ss[i].Part, nd(i) + nd(ss[i].Part) + nd(results[i]));
        for(; i < results.Count; i++)
            a_fail(nd(i) + nd(results[i]));
        for(; i < ss.Count; i++)
            a_fail(nd(i) + nd(ss[i].Part));
    }

    test_(Simple)
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

    test_(Text)
    {
        Test(" \"a_if\"\" b\" '\" ' ",
        {
            "",
            "\"a_if\"\" b\"",
            "'\" '",
            ""
        });
    }

    test_(Comment)
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

    test_(LineCommentError)
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

    test_(CommentError)
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

    test_(Pattern)
    {
        auto s = Source::FromText("asdf") + 0;

        auto p = SourceEnd.Find + HWLang::Error(String("x"));
        try
        {
            auto sp = p.Match(s);
        }
        catch(Exception<String> xxx)
        {
            a_if_(xxx.Position.IsEnd);
            a_if_(xxx.Error == "x");
        }
    }
}
