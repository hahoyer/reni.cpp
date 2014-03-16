#pragma once
#include "Type.h"
#include "Size.h"
#include "../HWLib/WeakRef.h"

using namespace HWLib;

namespace Reni{
    class Size;
    class Syntax;

    class NumberType final : public Type{
        typedef Type baseType;
        typedef NumberType thisType;
        class DumpPrintProvider;

        Ref<DumpPrintProvider> dumpPrintFeature;
    public:
        WeakRef<ArrayType> const parent;
        NumberType(WeakRef<ArrayType> const parent);
        AssumeConstObject;
    private:
        p_function(Size, size) override;
        p_function(Array<String>, DumpData) override;

        operator Ref<FeatureProvider<DumpPrintToken>, true>() const override;

        ResultData const DumpPrintResult(
            Context const&context,
            Category category,
            Ref<Syntax> target
            )const;
    };
}
