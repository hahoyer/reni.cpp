#include "Import.h"

#include "Reni.h"

static bool Trace = true;

namespace _Compiler
{
    class Syntax{};


    void RunAll()
    {
        auto m = PositionManager<Syntax>();
        auto pt = PrioTable();
        auto syntax = Parse(m, pt);
    }
}