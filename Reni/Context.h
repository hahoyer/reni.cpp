#pragma once
#include "TokenClass.h"
#include "../HWLib/RefCountProvider.h"

using namespace HWLib;

namespace Reni
{
    class Category;
    class ResultData;
    class Syntax;
    class RootContext;
    class SearchResult;

    class Context 
        : public WithId<DumpableObject, Context>
        , public RefCountProvider
    {
        using baseType = WithId<DumpableObject, Context>;
        using thisType = Context;
    public:
        ResultData const GetResultData(Category category, Syntax const&syntax)const;
        virtual_p(WeakRef<RootContext>, rootContext) = 0;
        SearchResult const Search(Ref<Syntax, true> const&left, TokenClass const&tokenClass)const;
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
