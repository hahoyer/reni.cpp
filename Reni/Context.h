#pragma once
#include "..\HWLib\DumpableObject.h"
#include "TokenClass.h"

using namespace HWLib;

namespace Reni
{
    class Category;
    class ResultData;
    class Syntax;
    class RootContext;
    class SearchResult;

    class Context : public WithId<DumpableObject, Context>
    {
        using baseType = WithId<DumpableObject, Context>;
        using thisType = Context;
    public:
        ResultData const GetResultData(Category category, Syntax const&syntax)const;
        virtual_p(RootContext const&, rootContext) = 0;
        SearchResult const Search(OptRef<Syntax> const&left, TokenClass const&tokenClass)const;
    };
}

namespace std {
    template <>
    struct hash<Reni::Context const&>{
        inline std::size_t operator()(Reni::Context const& key) const{
            return key.ObjectId;
        }
    };
}
