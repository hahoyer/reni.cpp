#pragma once

#include "bigInt.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/Common.h"

using namespace HWLib;

namespace Reni{
    class Size;

    class BitsConst final
        : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = BitsConst;
    public:
        using dataChunkType = BigInt::Ulong;
    private:
        BigInt::Rossi const value;
        BitsConst(String const&text) : value(text.RawData, 10){ SetDumpString(); }
    public:
        static BitsConst const Convert(String const&text);
        static int const MinusSize(int left, int right){ return max(left, right) + 1; }
        operator int const()const;

        p(Size, size);
        bool const operator==(BitsConst const&other)const{ return value == other.value; };
        p(String, format);
        p(Array<dataChunkType>, ToArray);
    private:
        p_function(Array<String>,DumpData) override{ return{ value.toStrDec() }; };
        p_function(String, DumpShort) override;
    };
}
