#pragma once

#include "AccessFeature.h"
#include "Global.h"

#if 0
#include "ContainerContext.h"
#include "FunctionCallContext.h"
#include "FunctionSyntax.h"
#include "SyntaxContainer.h"
#include "Type.h"
#include "../HWLib/ValueCache.h"
#include "../HWLib/FunctionCache.h"
#endif

namespace Reni
{
    class AccessData;
    class ContainerContext;
    class FunctionSyntax;

    class SimpleFeature final : public AccessFeature::Simple
    {
        typedef Simple baseType;
        typedef SimpleFeature thisType;

        Ref<AccessData> data;
    public:
        SimpleFeature(AccessData const&data);
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
        size_t const tokenIndex;

    public:
        ExtendedFeature(ContainerContext const& parent, size_t const tokenIndex)
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

        FunctionBodyType(Context const& context, FunctionSyntax const& body);

        ThisRef;

    private:
        p_function(Array<String>, DumpData) override
        {
            return{
                nd(context),
                nd(body)
            };
        };

        p_function(bool, hllw) { return true; };
        p_function(Size, size) override{ return 0; }
        p_function(WeakRef<Global>, global)override;
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
        p_function(bool, hllw) { return size == 0; };
        p_function(Size,size) override;
        p_function(WeakRef<Global>, global) override;
    };

};