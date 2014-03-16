#pragma once
#include "Type.h"

using namespace HWLib;

namespace Reni{
    class DumpPrintToken;

    class ArrayType final: public Type{
        using baseType = Type;
        using thisType = ArrayType;
    public:
        Type const& elementType;
        int const count;

        ArrayType(Type const& elementType, int count)
            : elementType(elementType)
              , count(count){
            SetDumpString();
        }

        AssumeConstObject;
    private:
        p_function(Size,size) override;
        p_function(Array<String>,DumpData) override{
            return{
                    nd(elementType),
                    nd(count)
                };
        }
    };
};