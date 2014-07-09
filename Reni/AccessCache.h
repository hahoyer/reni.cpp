#pragma once
#include "Result.h"

using namespace HWLib;

namespace Reni
{
    class AccessCache final : public ResultCache
    {
        using baseType = ResultCache;
        using thisType = AccessCache;
        ResultData const GetResultData(Category category)const override;
    };
}

