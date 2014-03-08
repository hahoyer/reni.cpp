#pragma once
#include "Type.h"

using namespace HWLib;

namespace Reni{
    class DumpPrintToken;

    class ArrayType final: public Type{
        using baseType = Type;
        using thisType = ArrayType;
        Type const& elementType;
        int const count;
    public:
        ArrayType(Type const& elementType, int count)
            : elementType(elementType)
              , count(count){
            SetDumpString();
        }

        ref_p;
    private:
        virtual operator Ref<FeatureProvider<DumpPrintToken>,true>()const override;
        override_p_function(Size, size);
        override_p_function(Array<String>, DumpData){
            return{
                    nd(elementType),
                    nd(count)
                };
        };
    };
};