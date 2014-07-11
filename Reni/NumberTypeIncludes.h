#pragma once

#include "Address.h"
#include "Feature.h"
#include "Result.h"
#include "Global.h"

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
            auto argSize = argTypeAsNumber->size.value;
            auto resultType = thisTypeAsNumber->ResultType<TTokenClass>(argSize);
            auto code = CodeItem::NumberOperation
                (
                TTokenClass::Text(),
                resultType->toAddress,
                target,
                arg
                );
            return resultType->GetResultDataSmartExts(category, l_(code));
        }

        md(category, target, arg);
        b_;
        return{};
    }

    static int const ResultSize(int target, int arg);
    static WeakRef<Type> const ResultType(NumberType const& target, int arg);
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
inline WeakRef<Type> const NumberType::ResultType<MinusToken>(int other)const
{
    return Resize(BitsConst::MinusSize(size.value, other));
}

template<>
inline WeakRef<Type> const NumberType::ResultType<PlusToken>(int other)const
{
    return Resize(BitsConst::PlusSize(size.value, other));
}

template<>
inline WeakRef<Type> const NumberType::ResultType<StarToken>(int other)const
{
    return Resize(BitsConst::TimesSize(size.value, other));
}

template<>
inline WeakRef<Type> const NumberType::ResultType<EqualToken>(int )const
{
    return global->boolType.thisRef;
}

template<>
inline WeakRef<Type> const NumberType::ResultType<GreaterToken>(int)const
{
    return global->boolType.thisRef;
}

