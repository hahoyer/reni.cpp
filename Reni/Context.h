#pragma once
#include "TokenClass.h"
#include "../HWLib/RefCountProvider.h"
#include "SearchResult.h"
#include "ContextFeature.h"
#include "ContextFeatureProvider.h"


using namespace HWLib;

namespace Reni
{
    class Category;
    class Global;
    class ResultData;
    class Syntax;
    class RootContext;
    class MinusToken;

    class Context 
        : public WithId<DumpableObject, Context>
        , public RefCountProvider
    {
        using baseType = WithId<DumpableObject, Context>;
        using thisType = Context;
    public:
        ResultData const GetResultData(Category category, Syntax const&syntax)const;
        virtual_p(WeakRef<Global>, global) = 0;
        SearchResult const Search(Ref<Syntax, true> const&left, TokenClass const&tokenClass)const;

        virtual operator Ref<ContextFeatureProvider<MinusToken>, true>()const;

        template<class T>
        SearchResult const GetGenericDefinition()const {
            Ref<ContextFeatureProvider<T>> f = *this;
            Ref<ContextFeature> ff = f->feature;
            return *ff;
        }
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
