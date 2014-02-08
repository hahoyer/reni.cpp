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
        auto pt = PrioTable();
        auto syntax = Parse(m, pt);
    }
}