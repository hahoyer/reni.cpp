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
            friend class NumberType;
            static ResultData const Result(
                NumberType const&type,
                Context const&context,
                Category category,
                Ref<Syntax> target
                );
        };

        typedef ArglessFunctionProvider<DumpPrintToken, DumpPrintProvider> dumpPrintProviderType;
        Ref<dumpPrintProviderType> dumpPrintFeature;
    public:
        WeakRef<ArrayType> const parent;
        NumberType(WeakRef<ArrayType> const parent);
        AssumeConstObject;
    private:
        p_function(Size, size) override;
        p_function(Array<String>, DumpData) override;

        operator Ref<FeatureProvider<DumpPrintToken>, true>() const override;

    };
}
