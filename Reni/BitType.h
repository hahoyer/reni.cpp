#pragma once
#include "Type.h"
#include "../HWLib/WeakRef.h"

using namespace Util;

namespace Reni
{
    class ArrayType;
    class DumpPrintToken;

    class BitType final : public Type
    {
        using baseType = Type;
        using thisType = BitType;
        WeakRef<Global> _global;
    public:
        explicit BitType(WeakRef<Global> global)
            : _global(global)
        {
            SetDumpString();
        }

        ThisRef;
    private:
        p_function(Size, size) override{ return 1; }
        p_function(WeakRef<Global>, global) override{ return _global; };
        p_function(bool, hllw) { return false; };
    };

    class BoolType final : public Type
    {
        using baseType = Type;
        using thisType = BoolType;
        WeakRef<Global> _global;
    public:
        explicit BoolType(WeakRef<Global> global)
            : _global(global)
        {
            SetDumpString();
        }

        ThisRef;
    private:
        p_function(Size, size) override{ return 1; }
        p_function(WeakRef<Global>, global) override{ return _global; };
        p_function(bool, hllw) { return false; };
    };
}