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
        ref_p;
    private:
        override_p_function(Size, size);
        virtual operator Ptr<FeatureProvider<DumpPrintToken, ArrayType>>()const override;
        virtual Ptr<FeatureProvider<DumpPrintToken>>const Convert(ArrayType const&top)const override;
    };
}
