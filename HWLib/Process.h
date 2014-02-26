#pragma once
#include "Properties.h"
#include "Ref.h"

namespace HWLib{
    class Process{
        class internal;
        Pointer<internal> _internal;
    public:
        Process(String const& command);
        p(String, data);
        p(String, errorData);
        p(int, result);
        void Execute();
    };

    String const Dump(Process const&target);
}