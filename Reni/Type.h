#pragma once
#include "..\HWLib\DumpableObject.h"
#include "TokenClass.h"
#include "SearchResult.h"
#include "BitsConst.h"

using namespace HWLib;

namespace Reni
{
    class Type: public WithId<DumpableObject, Type>
    {
        using baseType = WithId<DumpableObject, Type>;
        using thisType = Type;
        struct internal;
        Ref<internal> _internal;
    protected:
        Type();
    public:
        virtual_p(Size, size) = 0;
        SearchResult const Search(TokenClass const&tokenClass)const;
        Type const& array(int count)const;
        ResultData const GetResultData(Category category, BitsConst const&value)const;
        operator Type& ()const{ return const_cast<Type&>(*this); }
    private:
        override_p_function(Array<String>, DumpData){ return{}; };
    };
}
