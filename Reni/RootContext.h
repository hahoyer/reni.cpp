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

        p(BitType const&, bitType);
    private:
        override_p_function(RootContext const&, rootContext) { return *this; };
        override_p_function(Array<String>, DumpData);
    };

}
