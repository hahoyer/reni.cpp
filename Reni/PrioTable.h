#pragma once
#include "../HWLang/PrioTable.h"
using namespace HWLib;
using namespace HWLang;


namespace Reni
{
    struct PrioTable
    {
        static HWLang::PrioTable const Main()
        {
            return
                HWLang::PrioTable()
                .Left({Any})
                .Left({"<", ">", "<=", ">="})
                .Left({"=", "<>"})
                .Left({"*", "/"})
                .Left({"+", "-"})
                .Right({":="})
                .ThenElseLevel({"then"}, {"else"})
                .Left({"/\\", "/!\\", "/\\/\\", "/!\\/!\\"})
                .Right({":"})
                .Right({","})
                .Right({";"})
                .ParenthesisLevel({"(", "[", "{"}, {")", "]", "}"})
                .ParenthesisLevel({Start}, {End})
                ;
        };
    };
}