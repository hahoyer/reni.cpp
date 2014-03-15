#pragma once
#include "Type.h"

namespace Reni{
    class ArrayType;
    class DumpPrintToken;
    class Size;

    class BitType final : public Type{
        using baseType = Type;
        using thisType = BitType;
    public:

        BitType(){
            SetDumpString();
        }

        AssumeConstObject;

    private:
        p_function(Size,size) override;
        virtual operator Ref<FeatureProvider<DumpPrintToken, ArrayType>, true>()const override;
    };
}