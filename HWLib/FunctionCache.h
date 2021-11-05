#pragma once
#include "FunctionCacheBase.h"

namespace HWLib
{
    template <typename TValue, typename... TKey>
    class FunctionCache final 
        : public FunctionCacheBase<TValue, TKey...>{
        using baseType = FunctionCacheBase<TValue, TKey...>;
        using thisType = FunctionCache;
    public:
        FunctionCache(function<TValue(TKey...)> createValue)
            : baseType(createValue){};

        HW_DO_PLACEMENT_ASSIGN;
    };

};

