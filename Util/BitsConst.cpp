#include "Import.h"
#include "BitsConst.h"

#include "Size.h"

static bool Trace = true;

using namespace Util;

BitsConst const BitsConst::Empty(){
    return BitsConst();
}

BitsConst const BitsConst::Convert(String const&text){
    return BitsConst(text);
};

BitsConst::operator size_t const() const
{
    return value.toUlong();
}

p_implementation(BitsConst, Size, size){
    if (value == BigInt::Rossi(0))
        return 0;
    size_t result = (value.getUnitsSize() - 1) * 8;
    a_is(result, <, 10000);
    BigInt::Rossi value = this->value;
    if (result != 0)
        value >>= result;

    if (value < BigInt::Rossi(0))
        value = -value;
    else
        result++;

    while (value != BigInt::Rossi(0))
    {
        value >>= 1;
        result++;
    }
    return result;
}

p_implementation(BitsConst, String, format){
    return 
        Array<BigInt::Ulong>(value.ToArray())
        .Select<String>([](long element){return String::Convert(element); })
        ->Stringify(", ");
}


p_implementation(BitsConst, String, DumpShort){
    return String(value.toStrDec())
        + String(" size=") 
        + size.DumpShort;
};
