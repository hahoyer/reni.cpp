#include "Import.h"

#include "Reni.h"
#include "SimpleTokenFactory.h"

static bool Trace = true;

using namespace HWLang;
using namespace HWLang::PrioTableConst;


namespace _Compiler
{
    void RunAll()
    {
        Ref<Source const> s = new Source(Source::FromText("asdf"));

        auto pt = PrioTable::Left({ Any });
        pt = pt.ParenthesisLevel(Start, PrioTableConst::End);

        auto sc = _Compiler::ScannerInstance(s);

        auto syntax = Parse<_Compiler::Syntax<_Compiler::TokenClass>>(pt, sc);
        argdump(syntax);

    }
}