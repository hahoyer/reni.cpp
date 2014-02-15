#include "Import.h"

#include "Reni.h"

static bool Trace = true;

namespace _Compiler
{
    void RunAll()
    {
        Ref<Source const> s = new Source(Source::FromText("asdf"));
        auto tf = MainTokenFactory();
        auto m = PositionManager<Syntax>(s, tf, Token(tf.BeginOfText, SourcePart(s, 0, 0)));
        auto pt = PrioTable::Left({ PrioTableConst::Any });
        pt = pt.ParenthesisLevel(PrioTableConst::BeginOfText, PrioTableConst::EndOfText);

        auto syntax = Parse(m, pt);
        argdump(syntax);

    }
}