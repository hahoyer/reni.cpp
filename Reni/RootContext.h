#pragma once

#include "Context.h"

namespace Reni
{
    class BitType;

    class RootContext : public Context
    {
        using baseType = Context;
        using thisType = RootContext;
        Ref<BitType> const bitTypeInstance;
    public:
        RootContext();

        p(Ref<BitType>, bitType);
        ref_p;
    private:
        override_p_function(WeakRef<RootContext>, rootContext) { return &ref; };
        override_p_function(Array<String>, DumpData);
    };

}
