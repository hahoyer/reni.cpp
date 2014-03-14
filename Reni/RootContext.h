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
        WeakRef<internal> _internal;
    public:
        RootContext();
        p(WeakRef<BitType>, bitType);
        p(WeakRef<VoidType>, voidType);
        AssumeConstObject;
        virtual operator Ref<ContextFeatureProvider<MinusToken>, true>()const override;
    private:
        override_p_function(WeakRef<RootContext>, rootContext){return &ref;};
        override_p_function(Array<String>, DumpData);
    };
}