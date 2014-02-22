#pragma once

#include "bigInt.h"

namespace Reni{
    class Size;

    class BitsConst: public DumpableObject{
        using baseType = DumpableObject;
        using thisType = BitsConst;
        BigInt::Rossi const value;
        BitsConst(String const&text): value(text.RawData,10){}
    public:
        static Ref<BitsConst> const Convert(String const&text);
        p(Size, size);
        bool const operator==(BitsConst const&other)const{ return value == other.value; };
    private:
        override_p_function(Array<String>, DumpData){ return{ value.toStrDec() }; };
    };
}
