#pragma once
#include "Type.h"
#include "FeatureClass.h"

namespace Reni
{
    class ArrayType;
    class BitsConst;
    class Category;
    class DumpPrintToken;
    class ResultData;

    class BitType 
        : public Type
        , public FeatureProvider<DumpPrintToken, ArrayType>
    {
        using baseType = Type;
        using thisType = BitType;
    public:
        BitType(){ SetDumpString(); }
        virtual operator OptWeakRef<FeatureProvider<DumpPrintToken, ArrayType>>()const override;
        ref_p;
    private:
        override_p_function(Size, size);
        virtual OptWeakRef<FeatureProvider<DumpPrintToken>>const Convert(ArrayType const&top)const override;
    };
}
