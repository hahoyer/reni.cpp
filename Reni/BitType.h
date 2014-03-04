#pragma once
#include "Type.h"

namespace Reni
{
    class BitsConst;
    class Category;
    class ResultData;

    class BitType : public Type
    {
        using baseType = Type;
        using thisType = BitType;
        override_p_function(Size, size);
    public:
        BitType(){ SetDumpString(); }
    };
}
