#pragma once
#include "../HWLib/DumpableObject.h"

using namespace HWLib;

namespace Reni{

    class DefinitionPoint : public WithId<DumpableObject, DefinitionPoint>{
        using baseType = WithId<DumpableObject, DefinitionPoint>;
        using thisType = DefinitionPoint;
    public:
        ThisRef;
    private:
        p_function(Array<String>,DumpData) override{ return{}; };
    };

    template <typename T>
    class GenericDefinitionPoint : public DefinitionPoint{
        using baseType = DefinitionPoint;
        using thisType = GenericDefinitionPoint;
    };

}
