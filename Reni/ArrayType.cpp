#include "Import.h"
#include "ArrayType.h"

using namespace Reni;
static bool Trace = true;


HW_PR_IMPL_GETTER(ArrayType, Size, size){
    return elementType.size * count;
}
