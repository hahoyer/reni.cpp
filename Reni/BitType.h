#pragma once
#include "Type.h"
#include "../HWLib/WeakRef.h"

using namespace Util;

namespace Reni{
    class ArrayType;
    class DumpPrintToken;

    class BitType final : public Type{
        using baseType = Type;
        using thisType = BitType;
        WeakRef<Global> _global;
    public:
        BitType(WeakRef<Global> global)
        : _global(global){
            SetDumpString();
        }
        AssumeConstObject;
    private:
        p_function(Size,size) override;
        p_function(WeakRef<Global>, global) override{ return _global; };
    };
}