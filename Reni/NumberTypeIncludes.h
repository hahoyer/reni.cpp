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

    ResultData Result(Category const& category, Type const&target, Type const&arg)const override
    {
        auto thisTypeAsNumber = target.As<NumberType>();
        auto argTypeAsNumber = arg.As<NumberType>();
        if(!thisTypeAsNumber.IsEmpty && !argTypeAsNumber.IsEmpty)
        {
            auto argSize = argTypeAsNumber.Value->size.value;
            auto resultType = thisTypeAsNumber.Value->ResultType<TTokenClass>(argSize);
            auto code = CodeItem::NumberOperation
                (
                TTokenClass::Text(),
                resultType->toAddress,
                target,
                arg
                );
            return resultType->GetResultDataSmartClosure(category, l_(code));
        }

        md(category, target, arg);
        b_;
        return{};
    }

    static size_t const ResultSize(size_t target, size_t arg);
    static WeakRef<Type> const ResultType(NumberType const& target, size_t arg);
};


#include "DefineableToken.h"
#include "DumpPrintToken.h"

template<>
SearchResult<Feature> const NumberType::DeclarationsForType<EnableCutToken>() const
{
    return Feature::From<EnableCutFeature>(*this);
};

template<>
SearchResult<Feature> const NumberType::DeclarationsForType<DumpPrintToken>() const
{
    return Feature::From<DumpPrintFeature>(*this);
};

template<class TTokenClass>
SearchResult<Feature> const NumberType::DeclarationsForType() const
{
    return Feature::From<OperationFeature<TTokenClass>>(*this);
};

template<>
inline WeakRef<Type> const NumberType::ResultType<MinusToken>(size_t other)const
{
    return Resize(BitsConst::MinusSize(size.value, other));
}

template<>
inline WeakRef<Type> const NumberType::ResultType<PlusToken>(size_t other)const
{
    return Resize(BitsConst::PlusSize(size.value, other));
}

template<>
inline WeakRef<Type> const NumberType::ResultType<StarToken>(size_t other)const
{
    return Resize(BitsConst::TimesSize(size.value, other));
}

template<>
inline WeakRef<Type> const NumberType::ResultType<EqualToken>(size_t)const{return global->boolType.thisRef;}
template<>
inline WeakRef<Type> const NumberType::ResultType<GreaterToken>(size_t)const {return global->boolType.thisRef;}

