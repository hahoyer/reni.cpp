#include "Import.h"
#include "BitType.h"

#include "ArrayType.h"
#include "Size.h"

static bool Trace = true;

using namespace Reni;

override_p_implementation(BitType, Size, size){ return Size(1); }

BitType::operator OptRef<FeatureProvider<DumpPrintToken, ArrayType>>()const{ return &ref; }

OptRef<FeatureProvider<DumpPrintToken>>const BitType::Convert(ArrayType const&top)const{
    md(top);
    b_;
    return{};
}
