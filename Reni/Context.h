#pragma once
#include "TokenClass.h"
#include "SearchResult.h"
#include "ContextFeature.h"
#include "ContextFeatureProvider.h"

#include "../HWLib/RefCountProvider.h"
#include "../Util/Category.h"

using namespace HWLib;
using namespace Util;

namespace Reni{
    class FunctionSyntax;
    class FunctionType;
    class Global;
    class MinusToken;
    class ResultData;
    class RootContext;
    class Syntax;
    class SyntaxContainer;
    class DefineableToken;
    class FunctionCallContext;
    class RecursionContext;

    class Context
        : public WithId<DumpableObject, Context>{
        using baseType = WithId<DumpableObject, Context>;
        using thisType = Context;

    public:
        virtual_p(bool, isRecursion) { return false; };
        virtual_p(WeakRef<Global>, global) = 0;
        virtual_p(WeakRef<FunctionCallContext>, functionContext) = 0;

        SearchResult const Search(Ref<Syntax, true> const&left, DefineableToken const&tokenClass)const;
        virtual WeakRef<Type> const FunctionType(FunctionSyntax const& body) const = 0;
        ResultData ArgReferenceResult(Category category) const;
        virtual SearchResult const Search(DefineableToken const&token)const;
        virtual WeakRef<Context> const Container(SyntaxContainer const& syntax, int index) const = 0;
    };

    class RegularContext: public Context
    {
        using baseType = Context;
        using thisType = RegularContext;

        class internal;
        WeakRef<internal> _internal;
    public:
        RegularContext();

        p(WeakRef<RecursionContext>, recursionContext);

        WeakRef<Context> const Container(SyntaxContainer const& syntax, int index) const override;
        WeakRef<Type> const FunctionType(FunctionSyntax const& body) const override;

        virtual operator Ref<ContextFeatureProvider<MinusToken>, true>()const;
        virtual operator Ref<ContextFeatureProvider<DefineableToken>, true>()const;

        template<class T>
        SearchResult const GetGenericDefinition()const{
            Ref<ContextFeatureProvider<T>> f = *this;
            Ref<ContextFeature> ff = f->feature;
            return *ff;
        };

    };
}

namespace std{
    template <>
    struct hash<Reni::Context const&>{
        inline size_t operator()(Reni::Context const& key) const{
            return key.ObjectId;
        }
    };
}