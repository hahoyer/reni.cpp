#include "Import.h"
#include "ArrayType.h"

#include "Size.h"
#include "FeatureClass.h"
#include "FeatureProvider.h"

using namespace Reni;
static bool Trace = true;


override_p_implementation(ArrayType, Size, size){
    return elementType.size * count;
};

ArrayType::operator Ref<FeatureProvider<DumpPrintToken>,true>()const {
    Ref<FeatureProvider<DumpPrintToken, ArrayType>,true> f = elementType;
    if (f.IsValid)
        return f->Convert(*this);
    return{};
};


#include "../HWLib/RefCountContainer.instance.h"