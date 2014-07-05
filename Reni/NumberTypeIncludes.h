#pragma once

#include "Feature.h"
#include "Result.h"

using namespace Reni;

template<class TTokenClass>
class NumberType::OperationFeature final : public Feature::Extended
{
    using baseType = Extended;
    using thisType = OperationFeature;

    ResultData const Result(Category category, Type const&target, Type const&arg)const override
    {
        auto thisTypeAsNumber = target.As<NumberType>();
        auto argTypeAsNumber = arg.As<NumberType>();
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
            return resultType->GetResultDataSmartExts(category, l_(code));
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
SearchResult<Feature> const NumberType::DeclarationsForType<EnableCutToken>() const
{
    return Feature::From<EnableCutFeature>();
};

template<>
SearchResult<Feature> const NumberType::DeclarationsForType<DumpPrintToken>() const
{
    return Feature::From<DumpPrintFeature>();
};

template<class TTokenClass>
SearchResult<Feature> const NumberType::DeclarationsForType() const
{
    return Feature::From<OperationFeature<TTokenClass>>();
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