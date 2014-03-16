#pragma once

#include "Context.h"
#include "../HWLib/WeakRef.h"

namespace Reni{
    class BitType;
    class VoidType;

    class RootContext : public Context{
        typedef Context baseType;
        typedef RootContext thisType;
        class internal;
        CtrlRef<internal> _internal;
    public:
        RootContext();
        AssumeConstObject;

        p(WeakRef<BitType>, bitType);
        p(WeakRef<VoidType>, voidType);

        virtual operator Ref<ContextFeatureProvider<MinusToken>, true>()const override;
    private:
        p_function(WeakRef<RootContext>,rootContext) override{return &ref;};
        p_function(Array<String>,DumpData) override;
    };
}