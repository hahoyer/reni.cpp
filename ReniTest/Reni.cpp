#include "Import.h"
#include "Reni.h"

#include "CompilerTest.h"
#include "Parser.h"
#include "../HWLib/DumpToString.h"
#include "../Util/BitsConst.h"
#include "../Reni/CodeItems.h"

using namespace Util;
using namespace Reni;
using namespace ReniTest;

namespace TestReni
{
  HW_TM_IMPLEMENTATION(ParseSimple)
  {
    const CompilerTest ct ( "4711");
    const auto s = ct.compiler.syntax;
    HW_ASSERT(Dump(s)|BeginsWith("*{ Reni::TerminalSyntax<class Reni::NumberToken>.Id"), HW_D_VALUE(s));
  }

  HW_TM_IMPLEMENTATION(CodeSimple, ParseSimple)
  {
    const CompilerTest ct ("4711");
    auto s = ct.compiler.main;
    auto sc = dynamic_cast<const ConstCode*>(&*s);
    HW_ASSERT(sc, HW_D_VALUE(s));
    HW_ASSERT_IS(sc->size, ==, 14);
    const auto c4711 = BitsConst::Convert("4711");
    HW_ASSERT_IS(sc->value, ==, c4711);
  }

  HW_TM_IMPLEMENTATION(CompileSimple, CodeSimple)
  {
    CompilerTest ct ("4711");
    const auto result = ct.compiler.Execute();
    HW_ASSERT_IS(result.result, ==, 4711);
  }

  HW_TM_IMPLEMENTATION(Simple, CompileSimple)
  {
    CompilerTest::Check("3 dump_print", "3");
  }

  HW_TM_IMPLEMENTATION(Negative, Simple)
  {
    CompilerTest::Check("(-1)dump_print", "-1");
    CompilerTest::Check("(-12)dump_print", "-12");
    CompilerTest::Check("(-123)dump_print", "-123");
    CompilerTest::Check("(-1234)dump_print", "-1234");
    CompilerTest::Check("(-12345)dump_print", "-12345");
    CompilerTest::Check("(-123456)dump_print", "-123456");
    CompilerTest::Check("(-1234567)dump_print", "-1234567");
    CompilerTest::Check("(-12345678)dump_print", "-12345678");
    CompilerTest::Check("(-123456789)dump_print", "-123456789");
    CompilerTest::Check("(-1234567890)dump_print", "-1234567890");
  }

  HW_TM_IMPLEMENTATION(SimpleFunction, Simple)
  {
    CompilerTest::Check(R"(
            f: /\1; f() dump_print;
        )", "1");
  }

  HW_TM_IMPLEMENTATION(Function, SimpleFunction)
  {
    CompilerTest::Check(R"(
            f: /\ ^ + 1; f(2) dump_print;
        )", "3");
  }

  HW_TM_IMPLEMENTATION(SimpleFunctionWithNonLocal, Function)
  {
    CompilerTest::Check(R"(
            x: 100; f: /\x;f() dump_print;
        )", "100");
  }

  HW_TM_IMPLEMENTATION(PrimitiveRecursiveFunctionByteWithDump, Function, SimpleFunctionWithNonLocal, TestHWLang::ThenElse,
       TestHWLang::TrainWreck)
  {
    CompilerTest::Check(
      R"(i: <!> 10; f: /\ i > 0 then (i := (i - 1)enable_cut; i dump_print; f());f();)",
      "9876543210");
  }

  HW_TM_IMPLEMENTATION(RecursiveFunction, PrimitiveRecursiveFunctionByteWithDump)
  {
    CompilerTest::Check(R"(
f: /\
{
1000 type instance
    (
        {
            ^ = 1 then 1
            else
            (
                ^ * f
                [
                    ^ type instance((^ - 1)enable_cut)
                ]
            )
        }
        enable_cut
    )
};
f(4)dump_print

        )", "3");
  }
}

