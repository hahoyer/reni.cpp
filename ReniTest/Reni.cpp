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
        d(s);
        b_;
    }      
}

void _Reni::RunAll()
{
    ParseSimple();
    CodeSimple();
}