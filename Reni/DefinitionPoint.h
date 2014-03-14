#pragma once
#include "../HWLib/DumpableObject.h"

using namespace HWLib;

namespace Reni{
    class FeatureClass;
    class SearchResult;

    class DefinitionPoint : public WithId<DumpableObject, DefinitionPoint>{
        using baseType = WithId<DumpableObject, DefinitionPoint>;
        using thisType = DefinitionPoint;
    public:
        AssumeConstObject;
    private:
        override_p_function(Array<String>, DumpData){ return{}; };
    };

    template <typename T>
    class GenericDefinitionPoint : public DefinitionPoint{
        using baseType = DefinitionPoint;
        using thisType = GenericDefinitionPoint;
    };

}
