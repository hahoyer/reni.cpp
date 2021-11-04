#pragma once

#include "bigInt.h"
#include "Size.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/Common.h"

using namespace HWLib;

namespace Util{
    class BitsConst final
        : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = BitsConst;
    public:
        using dataChunkType = BigInt::Ulong;
    private:
        BigInt::Rossi const value;
        BitsConst(string const&text) : value(text.c_str(), 10){ SetDumpString(); }
        BitsConst() : value(0){ SetDumpString(); }
    public:
        static BitsConst Empty();
        static BitsConst Convert(string const&text);
        static size_t MinusSize(size_t left, size_t right) { return max(left, right) + 1; }
        static size_t PlusSize(size_t left, size_t right) { return max(left, right) + 1; }
        static size_t TimesSize(size_t left, size_t right)
        {
            if(left < right)
                return TimesSize(right, left);
            a_if_(left >= right);
            
            if(left == 0 || right == 0)
                return 0;
            if(left == 1)
                return 1;
            return left + right;
        }
        operator size_t const()const;

        p(Size, size);
        p(bool, isEmpty){ return !size.value; }
        bool operator==(BitsConst const& other) const { return value == other.value; };
        p(string, format);
        p(Array<dataChunkType>, ToArray);
    private:
        p_function(Array<string>,DumpData) override{ return{ value.toStrDec() }; };
        p_function(string, DumpShort) override;
    };
}
