#include "Import.h"
#include "ArrayType.h"

#include "Size.h"

using namespace Reni;
static bool Trace = true;


override_p_implementation(ArrayType, Size, size){
    return elementType.size * count;
};

ArrayType::operator OptRef<FeatureProvider<DumpPrintToken>>()const {
    OptRef<FeatureProvider<DumpPrintToken, ArrayType>> f = elementType;
    if (f.IsValid)
        return f->Convert(*this);
    return{};
};