#pragma once
#include "Type.h"

namespace Reni{

    class VoidType final : public Type{
        using baseType = Type;
        using thisType = VoidType;
        WeakRef<Global> _global;
    public:

        VoidType(WeakRef<Global> global)
            : _global(global){
            SetDumpString();
        }

        HW_PR_THISREF;

    private:
        HW_PR_DECL_GETTER(Size,size) override { return 0; };
        HW_PR_DECL_GETTER(WeakRef<Global>, global) override{ return _global; };
        HW_PR_DECL_GETTER(bool, hollow) { return true; };
    };
}