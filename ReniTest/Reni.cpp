#include "Import.h"
#include "Reni.h"
#include "CompilerTest.h"

static bool Trace = true;

using namespace Reni;
using namespace _Compiler;

namespace _Reni{

    void ParseSimple()
    {
        CompilerTest ct;
        ct.text = "4711";
        auto s = ct.compiler.syntax;
        a_if(s.Dump.BeginsWith("Ref{ Reni::TerminalSyntax.Id"), nd(s));
    }

    void CodeSimple()
    {
        CompilerTest ct;
        ct.text = "4711";
        auto s = ct.compiler.code;
        auto sc = dynamic_cast<ConstCode const*>(&*s);
        a_if(sc, nd(s));
        a_is(sc->size, == , 14);
        auto c4711 = BitsConst::Convert("4711");
        a_is(*sc->value, ==, *c4711);
    }      

    void CompileSimple()
    {
        CompilerTest ct;
        ct.text = "4711";
        ct.compiler.Execute();
        b_;
    }
}

void _Reni::RunAll()
{
    ParseSimple();
    CodeSimple();
    CompileSimple();
}