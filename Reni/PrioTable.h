#pragma once
#include "../HWLang/PrioTable.h"
using namespace HWLib;
using namespace HWLang;


namespace Reni{

    struct PrioTable{
        static HWLang::PrioTable const Main(){
            return
                HWLang::PrioTable::CreateLeft({Any})
                .Left({"arg"})
                .Left({"*", "/"})
                .Left({"+", "-"})
                .ParenthesisLevel({"(", "[", "{"}, {")", "]", "}"})
                .ParenthesisLevel(Start, End)
                ;
        };

    };
}