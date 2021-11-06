#include "Import.h"
#include "Scanner.h"

#include "SimpleTokenFactory.h"
#include "../HWLang/Source.h"
#include "../HWLang/Pattern.h"
#include "../HWLib/File.h"
#include "../Reni/Compiler.internal.h"
#include "../HWAnalyse/TestMacros.h"
#include "../HWAnalyse/TestFixture.h"

using namespace HWLang;
using namespace _HWLang;

namespace _HWLang{

    HW_TM_TEST(SimpleAndDetailed)
    {
        using TokenFactory = SimpleTokenFactory;
        using ScannerInstance = TokenFactory::ScannerInstance;
        using TokenClass = TokenFactory::TokenClass;

        auto file = File("1.reni");
        file.Data = " asd f";
        HW_CONSOLE.WriteLine(String::FilePosition(file.FullName, 1, 3, ""));
        auto sc = ScannerInstance(file);

        TokenClass const& start = sc.Step().Class;
        a_is(&start, == , &TokenFactory::Start);
        string t = sc.Step().Part;
        a_is(t, == , "asd");
    }

    void Test(string const&text, Array<string> results)
    {
        using TokenFactory = SimpleTokenFactory;
        using ScannerInstance = TokenFactory::ScannerInstance;

        auto sc = ScannerInstance(text);
        auto ss = sc.ToArray;
        auto i = 0;
        for(; i < results.Count && i < ss.Count; i++)
            a_if(results[i] == string(ss[i].Part), HW_D_VALUE(i) + HW_D_VALUE(ss[i].Part) + HW_D_VALUE(results[i]));
        for(; i < results.Count; i++)
            a_fail(HW_D_VALUE(i) + HW_D_VALUE(results[i]));
        for(; i < ss.Count; i++)
            a_fail(HW_D_VALUE(i) + HW_D_VALUE(ss[i].Part));
    }

    HW_TM_TEST(Simple)
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

    HW_TM_TEST(Text)
    {
        Test(" \"a_if\"\" b\" '\" ' ",
        {
            "",
            "\"a_if\"\" b\"",
            "'\" '",
            ""
        });
    }

    HW_TM_TEST(Comment)
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

    HW_TM_TEST(LineCommentError)
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

    HW_TM_TEST(CommentError)
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

    HW_TM_TEST(Pattern)
    {
        auto s = Source::FromText("asdf") + 0;

        auto p = SourceEnd.Find + HWLang::Error(string("x"));
        try
        {
            auto sp = p.Match(s);
        }
        catch(Exception<string> xxx)
        {
            a_if_(xxx.Position.IsEnd);
            a_if_(xxx.Error == "x");
        }
    }
}
