#pragma once
#include "..\HWLib\DumpableObject.h"
#include "TokenClass.h"
#include "SearchResult.h"
#include "BitsConst.h"

using namespace HWLib;

namespace Reni{
    class DefinitionPoint;

    class Type: public WithId<DumpableObject, Type>{
        using baseType = WithId<DumpableObject, Type>;
        using thisType = Type;
        struct internal;
        Ref<internal> _internal;
    protected:
        Type();
    public:
        virtual_p(Size, size) = 0;
        Type const& array(int count)const;
        ResultData const GetResultData(Category category, BitsConst const&value)const;
        virtual_p(Array<WeakRef<DefinitionPoint>>, DefinitionPoints);
        ref_p;
    private:
        override_p_function(Array<String>, DumpData){ return{}; };
    };
}
