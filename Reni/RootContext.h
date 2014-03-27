#pragma once

#include "Context.h"
#include "../HWLib/WeakRef.h"
#include "../HWLib/CtrlRef.h"

namespace Reni{
    class BitType;
    class VoidType;
    class Global;

    class RootContext : public Context{
        typedef Context baseType;
        typedef RootContext thisType;
        class internal;
        CtrlRef<internal> _internal;
        WeakRef<Global> _global;
    public:
        RootContext();
        AssumeConstObject;
        operator Ref<ContextFeatureProvider<MinusToken>, true>()const override;
        operator Ref<ContextFeatureProvider<UserDefinedToken>, true>()const override;
    private:
        p_function(WeakRef<Global>,global) override{return _global;};
        p_function(Array<String>,DumpData) override;
    };
}