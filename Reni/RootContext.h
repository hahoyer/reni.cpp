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
        HW_PR_THISREF;
    private:
        HW_PR_DECL_GETTER(WeakRef<FunctionCallContext>, functionContext) override{ HW_BREAK_AND_THROW; };
        HW_PR_DECL_GETTER(WeakRef<Global>, global) override{ return _global; };
        HW_PR_DECL_GETTER(Array<string>,DumpData) override;
    };
}