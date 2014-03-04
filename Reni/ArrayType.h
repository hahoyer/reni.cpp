#pragma once
#include "Type.h"
#include "../HWLib/WeakRef.h"
#include "FeatureClass.h"
#include "DumpPrintToken.h"

using namespace HWLib;

namespace Reni{
    class ArrayType final 
        : public Type
        , public FeatureProvider<DumpPrintToken>
    {
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
        virtual operator OptWeakRef<FeatureProvider<DumpPrintToken>>()const override;
        override_p_function(Size, size);
        override_p_function(Array<String>, DumpData){
            return{
                nd(elementType),
                nd(count)
            };
        };
    };
};
