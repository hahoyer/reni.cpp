#pragma once

#include "External.h"

#include "../HWLib/RefCountProvider.h"
#include "../Util/Category.h"
#include "../HWLib/WeakRef.h"
#include "../HWLib/Ref.h"
#include "../HWLib/Optional.h"

using namespace HWLib;
using namespace Util;

namespace Reni
{
    class AccessFeature;
    class ContainerContext;
    template <class T>class ContextFeatureProvider;
    class DefineableToken;
    class FunctionBodyType;
    class FunctionCallContext;
    class FunctionSyntax;
    class Global;
    class MinusToken;
    class RecursionContext;
    class ResultData;
    class RootContext;
    template <class T>class SearchResult;
    class Syntax;
    class SyntaxContainer;
    class Type;

    class Context
        : public WithId<DumpableObject, Context>
    {
        using baseType = WithId<DumpableObject, Context>;
        using thisType = Context;

    public:
        virtual_p(bool, isRecursion) { return false; };
        virtual_p(WeakRef<Global>, global) = 0;
        virtual_p(WeakRef<FunctionCallContext>, functionContext) = 0;
        virtual_p(WeakRef<RecursionContext>, recursionContext) = 0;
        p(int, alignBits) { return 3; };

        virtual WeakRef<Type> const FunctionType(FunctionSyntax const& body) const = 0;
        virtual ResultData const ReferenceResult(Category category, External::Function const& external) const;
        virtual SearchResult<AccessFeature> const DeclarationsForType(DefineableToken const&token)const;
        virtual WeakRef<ContainerContext> const Container(SyntaxContainer const& statements, size_t viewIndex) const = 0;
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

        WeakRef<ContainerContext> const Container(SyntaxContainer const& syntax, size_t index) const override;
        WeakRef<Type> const FunctionType(FunctionSyntax const& body) const override;

        virtual operator Optional<Ref<ContextFeatureProvider<MinusToken>>>()const;
        virtual operator Optional<Ref<ContextFeatureProvider<DefineableToken>>>()const;

        template<class T>
        SearchResult<AccessFeature>  const GetGenericDefinition()const{
            Ref<ContextFeatureProvider<T>> f = *this;
            Ref<AccessFeature> ff = f->feature;
            return *ff;
        };
    private:
        p_function(WeakRef<RecursionContext>, recursionContext) override;
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

        SearchResult<AccessFeature> const DeclarationsForType(DefineableToken const& token) const override;;
    private:
        p_function(WeakRef<Global>, global) override{ return parent.global; };
        p_function(WeakRef<FunctionCallContext>, functionContext) override{ return parent.functionContext; };
    };
}

using namespace Reni;

class External::Context final : public External
{
    using baseType = Reni::External;
    using thisType = Context;
    Reni::Context const& target;
public:
    Context(Reni::Context const& target)
        : target(target)
    {
        SetDumpString();
    }
    String const internalDump() const override;
private:
    Externals const Replace(ReplaceVisitor const& arg) const override;
};


namespace std{
    template <>
    struct hash<Reni::Context const&>{
        inline size_t operator()(Reni::Context const& key) const{
            return key.ObjectId;
        }
    };
}