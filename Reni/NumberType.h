#pragma once
#include "../HWLib/WeakRef.h"
#include "../Util/Size.h"

#include "DumpPrintToken.h"
#include "Feature.h"
#include "Result.h"
#include "Type.h"
#include <complex>

using namespace HWLib;
using namespace Util;

namespace Reni
{
    class Syntax;

    class NumberType final : public Type
    {
        typedef Type baseType;
        typedef NumberType thisType;
        ArrayType const& parent;
    public:
        explicit NumberType(WeakRef<ArrayType> const parent);
        NumberType(NumberType const&) = delete;
        ThisRef;

        template<class TTokenClass>
        SearchResult const Search() const;
        template<>
        SearchResult const Search<EnableCutToken>() const;

        WeakRef<NumberType> const Resize(int newSize)const;

        static WeakPtr<NumberType> const Convert(Type const&target);
    private:
        p_function(Size, size) override;
        p_function(WeakRef<Global>, global) override;
        p_function(Array<String>, DumpData) override;
        p_function(String, DumpShort) override;

        SearchResult const Search(DefineableToken const& token) const override;

        template<class TTokenClass>
        class OperationFeature final : public InfixFunctionFeature
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

    };
};

using namespace Reni;

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