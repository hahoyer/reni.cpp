#pragma once
#include "..\HWLib\DumpableObject.h"
#include "TokenClass.h"
#include "SearchResult.h"

using namespace HWLib;

namespace Reni
{
    class Type: public WithId<DumpableObject, Type>
    {
        using baseType = WithId<DumpableObject, Type>;
        using thisType = Type;
    public:
        SearchResult const Search(TokenClass const&tokenClass)const;
        Type const& array(int count)const;

    private:
        override_p_function(Array<String>, DumpData){ return{}; };
    };
}
