#pragma once
#include "../HWLib/DumpableObject.h"
#include "BitType.h"
#include "VoidType.h"

using namespace HWLib;

namespace Reni{
    class Functions final : public DumpableObject{
        typedef DumpableObject baseType; typedef Functions thisType;
    private:
        p_function(Array<String>, DumpData) override{ return{}; };
    };

    class Global: public DumpableObject{
        typedef DumpableObject baseType;
        typedef Global thisType;
    public:
        Global();
        ThisRef;

        BitType const bitType;
        VoidType const voidType;

        Functions functions;

    private:
        p_function(Array<String>, DumpData) override;
    };
}