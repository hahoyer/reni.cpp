#pragma once
#include "Type.h"
#include "Size.h"
#include "../HWLib/WeakRef.h"
#include "FeatureProvider.h"

using namespace HWLib;

namespace Reni{
    class Size;
    class Syntax;

    class NumberType final : public Type{
        typedef Type baseType;
        typedef NumberType thisType;

        struct DumpPrintProvider{
            typedef class NumberType targetType;
            static ResultData const Result(Category, NumberType const&type);
        };
        struct MinusProvider{
            typedef class NumberType targetType;
            static ResultData const Result(Category, NumberType const&_thisType, Type const&argType);
        };

        ArrayType const& parent;
    public:
        NumberType(WeakRef<ArrayType> const parent);
        AssumeConstObject;
    private:
        p_function(Size, size) override;
        p_function(WeakRef<Global>, global) override;
        p_function(Array<String>, DumpData) override;
        p_function(String, DumpShort) override;

        operator Ref<FeatureProvider<DumpPrintToken>, true>() const override{
            return new ArglessFunctionProvider<DumpPrintToken, DumpPrintProvider>(*this);
        };
        operator Ref<FeatureProvider<MinusToken>, true>() const override{
            return new InfixFunctionProvider<MinusToken, MinusProvider>(*this);
        };

        NumberType const& Resize(int newSize)const;
    };
}
