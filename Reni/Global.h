#pragma once
#include "../HWLib/DumpableObject.h"
#include "BitType.h"
#include "VoidType.h"

using namespace HWLib;

namespace Reni{
    class Global: public DumpableObject{
        typedef DumpableObject baseType;
        typedef Global thisType;
    public:
        Global();
        ThisRef;

        BitType const bitType;
        VoidType const voidType;
    private:
        p_function(Array<String>, DumpData) override;
    };
}