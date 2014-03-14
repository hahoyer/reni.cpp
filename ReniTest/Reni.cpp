#include "Import.h"
#include "Reni.h"
#include "CompilerTest.h"
#include "..\Reni\CodeItems.h"

static bool Trace = true;

using namespace Reni;
using namespace ReniTest;

void Common::RunAll()
{
    ParseSimple();
    CodeSimple();
    CompileSimple();

    Number::RunAll();
}

void Common::ParseSimple()
{
    CompilerTest ct = "4711";
    auto s = ct.compiler.syntax;
    a_if(Dump(s).BeginsWith("Ref{ Reni::TerminalSyntax<class Reni::NumberToken>.Id"), nd(s));
}

void Common::CodeSimple()
{
    CompilerTest ct= "4711";
    auto s = ct.compiler.code;
    auto sc = dynamic_cast<ConstCode const*>(&*s);
    a_if(sc, nd(s));
    a_is(sc->size, == , 14);
    auto c4711 = BitsConst::Convert("4711");
    a_is(sc->value, ==, c4711);
}

void Common::CompileSimple()
{
    CompilerTest ct= "4711";
    auto result = ct.compiler.Execute();
    a_is(result.result, == , 4711);
}

void Number::RunAll()
{
    Simple();
    Negative();
}

void Number::Simple()
{
    CompilerTest::Check("3 dump_print", "3");

}

void Number::Negative()
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
