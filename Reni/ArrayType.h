#pragma once
#include "Type.h"
#include "../HWLib/WeakRef.h"
#include "../Util/Size.h"

using namespace HWLib;

namespace Reni{
    class DumpPrintToken;

    class ArrayType final: public Type{
        using baseType = Type;
        using thisType = ArrayType;
    public:
        Type const& elementType;
        size_t const count;

        ArrayType(Type const& elementType, size_t count)
            : elementType(elementType)
              , count(count){
            SetDumpString();
        }

        ThisRef;
    private:
        p_function(Size,size) override;
        p_function(WeakRef<Global>, global) override{ return elementType.global; };
        p_function(Array<String>, DumpData) override{
            return{
                    nd(elementType),
                    nd(count)
                };
        }
        p_function(bool, hllw) { return false; };
    };
};