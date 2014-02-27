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
    a_if(s.Dump.BeginsWith("Ref{ Reni::TerminalSyntax.Id"), nd(s));
}

void Common::CodeSimple()
{
    CompilerTest ct= "4711";
    auto s = ct.compiler.code;
    auto sc = dynamic_cast<ConstCode const*>(&*s);
    a_if(sc, nd(s));
    a_is(sc->size, == , 14);
    auto c4711 = BitsConst::Convert("4711");
    a_is(*sc->value, ==, *c4711);
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
}

void Number::Simple()
{
    CompilerTest::Check("3 dump_print", "3");

}