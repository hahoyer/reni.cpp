#pragma once
#include "../HWLib/RefCountProvider.h"
#include "../HWLib/DumpableObject.h"
#include "../Util/Category.h"
#include "ArgVisitor.h"

using namespace HWLib;
using namespace Util;

namespace Reni{
    class ArrayType;
    class ExpressionSyntax;
    class ResultData;
    class Context;

    class Feature
        : public WithId<DumpableObject, Feature>
        , public RefCountProvider{
        typedef DumpableObject baseType;
        typedef Feature thisType;
    public:
        virtual ResultData const FunctionResult(
            Context const&context,
            Category category,
            ExpressionSyntax const& expressionSyntax
            )const;
    private:
        p_function(Array<String>,DumpData) override {
            return{};
        }
    };


    class InfixFunctionFeature : public Feature
    {
        typedef Feature baseType;
        typedef InfixFunctionFeature thisType;

        ResultData const FunctionResult(
            Context const&context,
            Category category,
            ExpressionSyntax const& expressionSyntax
            )const override;

    protected: 
        virtual ResultData const Result(Category category, Type const&target, Type const&arg) const = 0;
    };


    class SuffixFunctionFeature : public Feature
    {
        using baseType = Feature;
        using thisType = SuffixFunctionFeature;

        ResultData const FunctionResult(
            Context const&context,
            Category category,
            ExpressionSyntax const& expressionSyntax
            )const override;

    protected:
        virtual ResultData const Result(Category category, Type const&target) const = 0;
    };

    class EnableCutFeature final : public SuffixFunctionFeature
    {
        using baseType = InfixFunctionFeature;
        using thisType = EnableCutFeature;
        ResultData const Result(Category category, Type const&target)const override;
    };
}
                                                                    