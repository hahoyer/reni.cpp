#pragma once

#include "Context.h"
#include "../HWLib/WeakRef.h"

namespace Reni{
    class BitType;
    class VoidType;

    class RootContext : public Context{
        using baseType = Context;
        using thisType = RootContext;
        Ref<BitType> const bitTypeInstance;
        Ref<VoidType> const voidTypeInstance;
    public:
        RootContext();
        p(WeakRef<BitType>, bitType);
        p(WeakRef<VoidType>, voidType);
        ref_p;
    private:
        override_p_function(WeakRef<RootContext>, rootContext){return &ref;};
        override_p_function(Array<String>, DumpData);
    };
}