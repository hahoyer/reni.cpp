#pragma once
#include "Type.h"
#include "Size.h"

namespace Reni{

    class VoidType final : public Type{
        using baseType = Type;
        using thisType = VoidType;
    public:

        VoidType(){
            SetDumpString();
        }

        ref_p;

    private:
        override_p_function(Size, size) { return 0; };
    };
}