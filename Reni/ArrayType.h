#pragma once
#include "Type.h"
#include "../HWLib/WeakRef.h"
#include "FeatureClass.h"

using namespace HWLib;

namespace Reni{
    class ArrayType final : public Type{
        using baseType = Type;
        using thisType = ArrayType;
        Type const& elementType;
        int const count;

    public:
        ArrayType(Type const& elementType, int count)
            : elementType(elementType)
            , count(count){}
        GenericDefinitionPoint<thisType> definitionPoint;
    private:
        override_p_function(Array<WeakRef<DefinitionPoint>>, DefinitionPoints);
        override_p_function(Size, size);
        override_p_function(Array<String>, DumpData){
            return{
                nd(elementType),
                nd(count)
            };
        };
    };
};
