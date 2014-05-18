#pragma once

#include "FunctionContext.h"
#include "../HWLib/ValueCache.h"

namespace Reni{

    class DefinableTokenFeatureProvider final : public ContextFeatureProvider<DefineableToken>{
        typedef ContextFeatureProvider<DefineableToken> baseType;
        typedef DefinableTokenFeatureProvider thisType;

        class Feature final : public ContextFeature{
            typedef ContextFeature baseType;
            typedef Feature thisType;
        private:
            virtual ResultData const FunctionResult(
                Context const&context,
                Category category,
                Ref<Syntax, true> const& right
                )const override;
            p_function(Array<String>, DumpData) override{
                return{};
            }
        };

    public:
        ThisRef;

    private:
        p_function(Ref<ContextFeature>, feature) override{
            return new Feature();
        }

        p_function(Array<String>, DumpData) override{
            return{};
        }
    };


    class ContainerContext;

    class AccessFeature final : public Feature{
        typedef Feature baseType;
        typedef AccessFeature thisType;

    public:
        ContainerContext const& container;
    private:
        int const tokenIndex;
    public:
        AccessFeature(ContainerContext const& containerContext, int tokenIndex);
    private:
        p_function(Array<String>, DumpData) override;
        ResultData const FunctionResult(Context const& context, Category category, ExpressionSyntax const& expressionSyntax) const override;
    };

    class FunctionCallResultCache;

    class ContainerContext final : public ChildContext
    {
        typedef ContainerContext thisType;
        typedef ChildContext baseType;

        FunctionCache<Ref<FunctionCallResultCache>, Type const*, Syntax const*> functionCallResultCache;
        FunctionCache<Ref<Feature>, int> accessFeature;
        ValueCache<WeakRef<Type>> dataTypeCache;
    public:
        Ref<SyntaxContainer> containerData;
    private:
        int const index;
    public:
        ContainerContext(RegularContext const&parent, SyntaxContainer const&containerData, int index);

        ContainerContext(ContainerContext const&) = delete;
        ThisRef;

        p(WeakRef<Type>, dataType){ return dataTypeCache.Value; };
        p(Size, dataSize);

        Ref<FunctionCallResultCache> const FunctionCallResult(Type const& argsType, int const tokenIndex) const;

        SearchResult const Search(DefineableToken const&token) const override{
            if(containerData->names.ContainsKey(&token)){
                auto tokenIndex = containerData->names[&token];
                return accessFeature(tokenIndex);
            }
            return baseType::Search(token);
        }

    private:
        p_function(Array<String>, DumpData) override{
            return base_p_name(DumpData) +
            _({
                nd(containerData),
                nd(index)
            });
        };

        Ref<DefinableTokenFeatureProvider> const token;
        operator Ref<ContextFeatureProvider<DefineableToken>, true>() const override{return token->thisRef;}
    };

    class FunctionType final : public Type{
        typedef Type baseType;
        typedef FunctionType thisType;
        Context const& context;
        Ref<FunctionSyntax> body;
    public:

        FunctionType(Context const& context, FunctionSyntax const& body)
            : context(context)
            , body(body.thisRef){
        }

        ThisRef;

    private:
        p_function(Array<String>, DumpData) override{
            return{
                nd(context),
                nd(body)
            };
        };

        p_function(Size, size) override{return 0;}
        p_function(WeakRef<Global>, global) override{return context.global;};
    };


    class FunctionCallResultCache final : public ResultCache{
        typedef ResultCache baseType;
        typedef FunctionCallResultCache thisType;
        Syntax const&body;
        FunctionCallContext context;
    public:
        FunctionCallResultCache(ContainerContext const& container, Type const*args, Syntax const&body)
            : context(container, args)
            , body(body.thisRef){
            SetDumpString();
        }

    private:
        p(WeakPtr<Type const>, args){ return context.args; }
        p(ContainerContext const&, container){ return context.container; }
        p_function(Array<String>, DumpData) override{ return{nd(body), nd(context)}; }

        ResultData const GetResultData(Category category)const override;
    };


    class ContainerType final : public Type{
        typedef Type baseType; 
        typedef ContainerType thisType;
        ContainerContext const&parent;
    public:
        ContainerType(ContainerContext const&parent) : parent(parent){
            SetDumpString();
        }
    private:
        p_function(Array<String>, DumpData) override{ return{nd(parent)}; }
        p_function(Size, size) override{ return parent.dataSize; }
        p_function(WeakRef<Global>, global) override{ return parent.global; };
    };
};


