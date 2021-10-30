#pragma once
#include "Properties.h"
#include "CtrlRef.h"

namespace HWLib{
    class Process{
        class internal;
        CtrlRef<internal> _internal;
    public:
        Process(String const& command);
        Process(const String& applicationName, const String& command);
        p(String, data);
        p(String, errorData);
        p(int, result);
        void Execute();
    };

    String const Dump(Process const&target);
}