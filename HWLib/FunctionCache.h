#pragma once
#include "FunctionCacheBase.h"

namespace HWLib
{
    template <typename TKey, typename TValue>
    class FunctionCache final : public FunctionCacheBase<TKey, TValue>{
        using baseType = FunctionCacheBase<TKey, TValue>;
        using thisType = FunctionCache;
    public:
        FunctionCache(function<TValue(TKey)> createValue)
            : baseType(createValue){};

        DefaultAssignmentOperator;
    };

};