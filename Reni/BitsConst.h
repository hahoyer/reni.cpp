#pragma once

#include "bigInt.h"

namespace Reni
{
    class BitsConst: public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = BitsConst;

        BigInt::Rossi const value;

        BitsConst(String const&text)
            : value(text.RawData,10)
        {
        }
    public:
        static Ref<BitsConst> const Convert(String const&text);

    private:
        p(int, count);

        override_p_function(Array<String>, DumpData){ return{}; };
    };
}
