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
using namespace TestHWLang;

namespace TestHWLang
{
  HW_TM_IMPLEMENTATION(SimpleAndDetailed)
  {
    using TokenFactory = SimpleTokenFactory;
    using ScannerInstance = TokenFactory::ScannerInstance;
    using TokenClass = TokenFactory::TokenClass;

    auto file = File("1.reni");
    file.Data = " asd f";
    HW_CONSOLE.WriteLine(String::FilePosition(file.FullName, 1, 3, ""));
    auto sc = ScannerInstance(file);

    TokenClass const& start = sc.Step().Class;
    HW_ASSERT_IS(&start, ==, &TokenFactory::Start);
    string t = sc.Step().Part;
    HW_ASSERT_IS(t, ==, "asd");
  }

  void Test(string const& text, Array<string> results)
  {
    using TokenFactory = SimpleTokenFactory;
    using ScannerInstance = TokenFactory::ScannerInstance;

    auto sc = ScannerInstance(text);
    auto ss = sc.ToArray;
    auto i = 0;
    for(; i < results.Count && i < ss.Count; i++)
      HW_ASSERT(results[i] == string(ss[i].Part), HW_D_VALUE(i) + HW_D_VALUE(ss[i].Part) + HW_D_VALUE(results[i]));
    for(; i < results.Count; i++)
      HW_FAIL(HW_D_VALUE(i) + HW_D_VALUE(results[i]));
    for(; i < ss.Count; i++)
      HW_FAIL(HW_D_VALUE(i) + HW_D_VALUE(ss[i].Part));
  }

  HW_TM_IMPLEMENTATION(Simple)
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

  HW_TM_IMPLEMENTATION(Text)
  {
    Test(" \"HW_ASSERT\"\" b\" '\" ' ",
         {
           "",
           "\"HW_ASSERT\"\" b\"",
           "'\" '",
           ""
         });
  }

  HW_TM_IMPLEMENTATION(Comment)
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

  HW_TM_IMPLEMENTATION(LineCommentError)
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

  HW_TM_IMPLEMENTATION(CommentError)
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

  HW_TM_IMPLEMENTATION(Pattern)
  {
    auto s = Source::FromText("asdf") + 0;

    auto p = SourceEnd.Find + HWLang::Error(string("x"));
    try
    {
      auto sp = p.Match(s);
    }
    catch(Exception<string> xxx)
    {
      HW_ASSERT_(xxx.Position.IsEnd);
      HW_ASSERT_(xxx.Error == "x");
    }
  }
}
