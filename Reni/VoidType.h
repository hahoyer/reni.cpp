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

        ThisRef;

    private:
        p_function(Size,size) override { return 0; };
        p_function(WeakRef<Global>, global) override{ return _global; };
    };
}