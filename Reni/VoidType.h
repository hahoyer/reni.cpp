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

        AssumeConstObject;

    private:
        p_function(Size,size) override { return 0; };
    };
}