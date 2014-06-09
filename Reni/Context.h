#pragma once
#include "TokenClass.h"
#include "SearchResult.h"
#include "ContextFeature.h"
#include "ContextFeatureProvider.h"

#include "../HWLib/RefCountProvider.h"
#include "../Util/Category.h"
#include "External.h"

using namespace HWLib;
using namespace Util;

namespace Reni{
    class FunctionSyntax;
    class FunctionBodyType;
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

        virtual WeakRef<Type> const FunctionType(FunctionSyntax const& body) const = 0;
        virtual ResultData const ReferenceResult(Category category, External::Function const& external) const;
        virtual SearchResult<ContextFeature> const Search(DefineableToken const&token)const;
        virtual WeakRef<Context> const Container(SyntaxContainer const& syntax, int index) const = 0;
    };

    class RegularContext: public Context
    {
        using baseType = Context;
        using thisType = RegularContext;

        class internal;
        WeakRef<internal> _internal;

        RegularContext(RegularContext const&) = delete;
    protected:
        RegularContext();

    public:
        p(WeakRef<RecursionContext>, recursionContext);

        WeakRef<Context> const Container(SyntaxContainer const& syntax, int index) const override;
        WeakRef<Type> const FunctionType(FunctionSyntax const& body) const override;

        virtual operator Optional<Ref<ContextFeatureProvider<MinusToken>>>()const;
        virtual operator Optional<Ref<ContextFeatureProvider<DefineableToken>>>()const;

        template<class T>
        SearchResult<ContextFeature>  const GetGenericDefinition()const{
            Ref<ContextFeatureProvider<T>> f = *this;
            Ref<ContextFeature> ff = f->feature;
            return *ff;
        };

    };

    class ChildContext : public RegularContext
    {
        using baseType = RegularContext;
        using thisType = ChildContext;

        explicit ChildContext(ChildContext const&) = delete;
    protected:
        RegularContext const&parent;

        ChildContext(RegularContext const& parent)
            : parent(parent)
        {
        }

        p_function(Array<String>, DumpData) override{ return{nd(parent)}; }
        SearchResult<ContextFeature>  const Search(DefineableToken const& token) const override{ return parent.Search(token); };
    private:
        p_function(WeakRef<Global>, global) override{ return parent.global; };
        p_function(WeakRef<FunctionCallContext>, functionContext) override{ return parent.functionContext; };
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