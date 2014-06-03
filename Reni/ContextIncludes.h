#pragma once

#include "FunctionContext.h"
#include "FunctionSyntax.h"
#include "SyntaxContainer.h"
#include "Type.h"
#include "../HWLib/ValueCache.h"
#include "../HWLib/FunctionCache.h"

namespace Reni
{
    class ContainerContext;

    class SimpleFeature final : public ContextFeature::Simple
    {
        typedef Simple baseType;
        typedef SimpleFeature thisType;

        ContainerContext const&parent;
        int const tokenIndex;

    public:
        SimpleFeature(ContainerContext const& parent, int const tokenIndex)
            : parent(parent), tokenIndex(tokenIndex) {}
    private:
        ResultData const Result(
            Context const&context,
            Category category
        )const override;
    };

    class ExtendedFeature final : public ContextFeature::Extended
    {
        typedef Extended baseType;
        typedef ExtendedFeature thisType;

        ContainerContext const&parent;
        int const tokenIndex;

    public:
        ExtendedFeature(ContainerContext const& parent, int const tokenIndex)
            : parent(parent), tokenIndex(tokenIndex) {}
    private:
        ResultData const Result(
            Context const&context,
            Category category,
            Type const& right
        )const override;
    };


    class FunctionCallResultCache;

    class ContainerContext final : public ChildContext
    {
        typedef ContainerContext thisType;
        typedef ChildContext baseType;

        FunctionCache<Ref<FunctionCallResultCache>, Type const*, FunctionSyntax const*> functionCallResultCache;
        FunctionCache<ContextFeature, int> accessFeature;
        ValueCache<WeakRef<Type>> dataTypeCache;
    public:
        Ref<SyntaxContainer> containerData;
    private:
        int const index;
    public:
        ContainerContext(RegularContext const&parent, SyntaxContainer const&containerData, int index);

        ContainerContext(ContainerContext const&) = delete;
        ThisRef;

        p(WeakRef<Type>, dataType)
        {
            return dataTypeCache.Value;
        };

        p(Size, dataSize);

        Ref<FunctionCallResultCache> const FunctionCallResult(Type const& argsType, int const tokenIndex) const;

        SearchResult<ContextFeature> const Search(DefineableToken const&token) const override
        {
            if(containerData->names.ContainsKey(&token))
            {
                auto tokenIndex = containerData->names[&token];
                return accessFeature(tokenIndex);
            }
            return baseType::Search(token);
        }

    private:
        p_function(Array<String>, DumpData) override
        {
            return base_p_name(DumpData) +
                _({
                    nd(containerData),
                    nd(index)
                });
        };

    };

    class FunctionBodyType final : public Type
    {
        typedef Type baseType;
        typedef FunctionBodyType thisType;
        Context const& context;
        Ref<FunctionSyntax> body;
    public:

        FunctionBodyType(Context const& context, FunctionSyntax const& body)
            : context(context)
              , body(body.thisRef)
        {
        }

        ThisRef;

    private:
        p_function(Array<String>, DumpData) override
        {
            return{
                nd(context),
                nd(body)
            };
        };

        p_function(Size, size) override{return 0;}
        p_function(WeakRef<Global>, global) override{return context.global;};
    };


    class FunctionCallResultCache final : public ResultCache
    {
        typedef ResultCache baseType;
        typedef FunctionCallResultCache thisType;
        FunctionSyntax const&body;
        FunctionCallContext context;
    public:
        FunctionCallResultCache(ContainerContext const& container, Type const*args, FunctionSyntax const&body)
            : context(container, args)
              , body(body.thisRef)
        {
            SetDumpString();
        }

    private:
        p(WeakPtr<Type const>, args){return context.args;}
        p(ContainerContext const&, container){return context.container;}
        p_function(Array<String>, DumpData) override{return{nd(body), nd(context)};}

        ResultData const GetResultData(Category category)const override;
        p(Ref<CodeItem>, codeGet);
        p(Ref<CodeItem>, CodeSet);
        p(WeakRef<Type>, valueType);
        p(WeakRef<Type>, valueTypeInRecursion);
        p(Ref<CodeItem>, codeGetter);
        p(Ref<CodeItem>, CodeSetter);
    };


    class ContainerType final : public Type
    {
        typedef Type baseType;
        typedef ContainerType thisType;
        ContainerContext const&parent;
    public:
        ContainerType(ContainerContext const&parent) : parent(parent)
        {
            SetDumpString();
        }

    private:
        p_function(Array<String>, DumpData) override
        {
            return{nd(parent)};
        }

        p_function(Size, size) override
        {
            return parent.dataSize;
        }

        p_function(WeakRef<Global>, global) override
        {
            return parent.global;
        };
    };
};