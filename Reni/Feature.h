#pragma once
#include "../HWLib/RefCountProvider.h"
#include "../HWLib/DumpableObject.h"
#include "../Util/Category.h"
#include "../HWLib/Ref.h"

using namespace HWLib;
using namespace Util;

namespace Reni
{
    class ArrayType;
    class Context;
    class ExpressionSyntax;
    class ResultData;
    class ResultFromSyntaxAndContext;
    class Type;

    class Feature final
        : public WithId<DumpableObject, Feature>
    {
        typedef DumpableObject baseType;
        typedef Feature thisType;
    public:
        template<class T>
        static Feature const From(){ return Feature(*new T); }

        class Simple : public RefCountProvider
        {
            using thisType = Simple;
        public:
            ThisRef;
            virtual ResultData const Result(Category category, Type const&target) const = 0;
        };


        class Extended : public RefCountProvider
        {
            using thisType = Extended;
        public:
            ThisRef;
            virtual ResultData const Result(Category category, Type const&target, Type const&arg) const = 0;
        };


        Ref<Simple, true> const simple;
        Ref<Extended, true> const extended;

        Feature(){};

    private:
        explicit Feature(Extended const& extended)
            : extended(extended.thisRef)
        {
        }

        explicit Feature(Simple const& simple)
            : simple(simple.thisRef)
        {
        }

    public:
        ResultData const FunctionResult(
            Context const&context,
            Category category,
            ExpressionSyntax const& expressionSyntax
        )const;

        p(bool, isEmpty){return simple.IsEmpty && extended.IsEmpty;}

    private:
        p_function(Array<String>, DumpData) override{ return{nd(simple) + nd(extended)}; }
        ResultData const Result(Category category, Type const&target, Ref<ResultFromSyntaxAndContext, true> argResult) const;
    };


    class EnableCutFeature final : public Feature::Simple
    {
        using baseType = Simple;
        using thisType = EnableCutFeature;
        ResultData const Result(Category category, Type const&target)const override;
    };

    class DumpPrintFeature final : public Feature::Simple
    {
        using baseType = Simple;
        using thisType = DumpPrintFeature;
        ResultData const Result(Category category, Type const&target)const override;
    };
}