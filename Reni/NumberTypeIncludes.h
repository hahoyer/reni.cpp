#pragma once

#include "Feature.h"
#include "Result.h"

using namespace Reni;

template<class TTokenClass>
class NumberType::OperationFeature final : public InfixFunctionFeature
{
    using baseType = InfixFunctionFeature;
    using thisType = OperationFeature;

    ResultData const Result(Category category, Type const&target, Type const&arg)const override
    {
        auto thisTypeAsNumber = Convert(target);
        auto argTypeAsNumber = Convert(arg);
        if(!thisTypeAsNumber.IsEmpty && !argTypeAsNumber.IsEmpty)
        {
            auto thisSize = thisTypeAsNumber->size.value;
            auto argSize = argTypeAsNumber->size.value;
            auto resultType = thisTypeAsNumber->Resize(ResultSize(thisSize, argSize));
            auto code = CodeItem::BinaryOperation
                (
                TTokenClass::Text(),
                *resultType,
                *thisTypeAsNumber, target.addressLevel,
                *argTypeAsNumber, arg.addressLevel
                );
            return resultType->GetResultData(code);
        }

        md(category, target, arg);
        b_;
        return{};
    }

    static int const ResultSize(int target, int arg);
};


#include "DefineableToken.h"
#include "DumpPrintToken.h"

template<>
SearchResult const NumberType::Search<EnableCutToken>() const
{
    return new EnableCutFeature;
};


template<class TTokenClass>
SearchResult const NumberType::Search() const
{
    return new OperationFeature<TTokenClass>;
};

template<>
inline int const NumberType::OperationFeature<MinusToken>::ResultSize(int target, int arg)
{
    return BitsConst::MinusSize(target, arg);
}

template<>
inline int const NumberType::OperationFeature<PlusToken>::ResultSize(int target, int arg)
{
    return BitsConst::PlusSize(target, arg);
}

template<>
inline int const NumberType::OperationFeature<StarToken>::ResultSize(int target, int arg)
{
    return BitsConst::TimesSize(target, arg);
}