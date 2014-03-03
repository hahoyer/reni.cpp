#include "Import.h"
#include "ArrayType.h"

#include "Size.h"

using namespace Reni;
static bool Trace = true;


override_p_implementation(ArrayType, Array<WeakRef<DefinitionPoint>>, DefinitionPoints){

    Array<WeakRef<DefinitionPoint>> left = (baseType::virtual_p_name(DefinitionPoints)());
    WeakRef<DefinitionPoint> right = &definitionPoint.ref;
    return left+right;
}

override_p_implementation(ArrayType, Size, size){
    return elementType.size * count;
};
