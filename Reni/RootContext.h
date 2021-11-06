#pragma once

#include "Context.h"
#include "../HWLib/WeakRef.h"
#include "../HWLib/CtrlRef.h"

namespace Reni
{
    class BitType;
    class VoidType;
    class Global;

    class RootContext : public RegularContext
    {
        typedef RegularContext baseType;
        typedef RootContext thisType;
        class internal;
        CtrlRef<internal> _internal;
        WeakRef<Global> _global;
    public:
        RootContext();
        ThisRef;
    private:
        p_function(WeakRef<FunctionCallContext>, functionContext) override{ HW_BREAK_AND_THROW; };
        p_function(WeakRef<Global>, global) override{ return _global; };
        p_function(Array<string>,DumpData) override;
    };
}