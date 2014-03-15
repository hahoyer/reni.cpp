#pragma once

#include "bigInt.h"
#include "../HWLib/DumpableObject.h"

using namespace HWLib;

namespace Reni{
    class Size;

    class BitsConst
        : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = BitsConst;
    public:
        using dataChunkType = BigInt::Ulong;
    private:
        BigInt::Rossi const value;
        BitsConst(String const&text): value(text.RawData,10){}
    public:
        static BitsConst const Convert(String const&text);
        operator int const()const;

        p(Size, size);
        bool const operator==(BitsConst const&other)const{ return value == other.value; };
        p(String, format);
        p(Array<dataChunkType>, ToArray);
    private:
        p_function(Array<String>,DumpData) override{ return{ value.toStrDec() }; };
    };
}
