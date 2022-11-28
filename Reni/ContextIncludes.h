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
        HW_PR_THISREF;
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
        HW_PR_THISREF;
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

        HW_PR_THISREF;

    private:
        HW_PR_DECL_GETTER(Array<string>, DumpData) override
        {
            return{
                HW_D_VALUE(context),
                HW_D_VALUE(body)
            };
        };

        HW_PR_DECL_GETTER(bool, hollow) { return true; };
        HW_PR_DECL_GETTER(Size, size) override{ return 0; }
        HW_PR_DECL_GETTER(WeakRef<Global>, global)override;
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
        HW_PR_DECL_GETTER(Array<string>, DumpData) override{return{HW_D_VALUE(parent)};}
        HW_PR_DECL_GETTER(bool, hollow) { return size == 0; };
        HW_PR_DECL_GETTER(Size,size) override;
        HW_PR_DECL_GETTER(WeakRef<Global>, global) override;
    };

};