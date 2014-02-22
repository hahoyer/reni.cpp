#include "Import.h"
#include "BitsConst.h"

#include "Size.h"

static bool Trace = true;

using namespace Reni;

Ref<BitsConst> const BitsConst::Convert(String const&text){
    return new BitsConst(text);
};


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
