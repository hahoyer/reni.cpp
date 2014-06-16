#pragma once
#include "../HWLib/DumpableObject.h"
#include "BitType.h"
#include "VoidType.h"
#include "../HWLib/FunctionCache.h"

using namespace HWLib;

namespace Reni
{
    class FunctionCallResultCache;

    class Global: public DumpableObject
    {
        typedef DumpableObject baseType;
        typedef Global thisType;
        static int nextIndex;

        FunctionCache<int, FunctionCallResultCache const*> codeIndexCache;
    public:
        Global();
        ThisRef;

        BitType const bitType;
        VoidType const voidType;
        int const CodeIndex(FunctionCallResultCache const& target)const;
    private:
        p_function(Array<String>, DumpData) override;
    };
}