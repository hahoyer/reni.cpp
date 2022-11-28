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

        HW_PR_THISREF;
    private:
        HW_PR_DECL_GETTER(Size, size) override{ return 1; }
        HW_PR_DECL_GETTER(WeakRef<Global>, global) override{ return _global; };
        HW_PR_DECL_GETTER(bool, hollow) { return false; };
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

        HW_PR_THISREF;
    private:
        HW_PR_DECL_GETTER(Size, size) override{ return 1; }
        HW_PR_DECL_GETTER(WeakRef<Global>, global) override{ return _global; };
        HW_PR_DECL_GETTER(bool, hollow) { return false; };
    };
}