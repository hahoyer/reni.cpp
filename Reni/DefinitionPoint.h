#pragma once
#include "../HWLib/DumpableObject.h"

using namespace HWLib;

namespace Reni{

    class DefinitionPoint : public WithId<DumpableObject, DefinitionPoint>{
        using baseType = WithId<DumpableObject, DefinitionPoint>;
        using thisType = DefinitionPoint;
    public:
        HW_PR_THISREF;
    private:
        HW_PR_DECL_GETTER(Array<string>,DumpData) override{ return{}; };
    };

    template <typename T>
    class GenericDefinitionPoint : public DefinitionPoint{
        using baseType = DefinitionPoint;
        using thisType = GenericDefinitionPoint;
    };

}
