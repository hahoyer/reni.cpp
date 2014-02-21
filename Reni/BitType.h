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
    public:
        ResultData const GetResultData(Category category, Ref<BitsConst> value)const;
    };
}
