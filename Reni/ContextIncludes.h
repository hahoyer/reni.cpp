#pragma once

#include "ContainerContext.h"
#include "FunctionCallContext.h"
#include "FunctionSyntax.h"
#include "SyntaxContainer.h"
#include "Type.h"
#include "../HWLib/ValueCache.h"
#include "../HWLib/FunctionCache.h"

namespace Reni
{
    class ContainerContext;

    class SimpleFeature final : public AccessFeature::Simple
    {
        typedef Simple baseType;
        typedef SimpleFeature thisType;

        Ref<ResultCache> resultCache;
        int const statementIndex;

    public:
        SimpleFeature(ContainerContext const& parent, int const tokenIndex);
        ThisRef;
    private:
        ResultData const Result(
            Context const&context,
            Category category
        )const override;
    };

    class ExtendedFeature final : public AccessFeature::Extended
    {
        typedef Extended baseType;
        typedef ExtendedFeature thisType;

        ContainerContext const&parent;
        int const tokenIndex;

    public:
        ExtendedFeature(ContainerContext const& parent, int const tokenIndex)
            : parent(parent), tokenIndex(tokenIndex) {}
        ThisRef;
    private:
        ResultData const Result(
            Context const&context,
            Category category,
            Type const& right
        )const override;
    };


    class FunctionCallResultCache;

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

        p_function(bool, isData) { return false; };
        p_function(Size, size) override{ return 0; }
        p_function(WeakRef<Global>, global) override{return context.global;};
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
        p_function(Array<String>, DumpData) override{return{nd(parent)};}
        p_function(bool, isData) { return size > 0; };
        p_function(Size, size) override{ return parent.dataSize; }
        p_function(WeakRef<Global>, global) override{return parent.global;};
    };
};