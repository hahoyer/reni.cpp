#include "Import.h"
#include "ArrayType.h"

#include "FeatureProvider.h"
#include "../HWLib/RefCountContainer.instance.h"

using namespace Reni;
static bool Trace = true;


p_implementation(ArrayType, Size, size){
    return elementType.size * count;
}
