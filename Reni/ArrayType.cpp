#include "Import.h"
#include "ArrayType.h"

#include "Size.h"

using namespace Reni;
static bool Trace = true;


override_p_implementation(ArrayType, Size, size){
    return elementType.size * count;
};

ArrayType::operator OptWeakRef<FeatureProvider<DumpPrintToken>>()const {
    auto e = elementType.operator OptWeakRef<FeatureProvider<DumpPrintToken, ArrayType>>();
    if (e.IsValid)
        return e.Convert(count);
};