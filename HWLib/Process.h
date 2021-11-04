#pragma once
#include "Properties.h"
#include "CtrlRef.h"

namespace HWLib{
    class Process{
        class internal;
        CtrlRef<internal> _internal;
    public:
        Process(std::string const& command);
        Process(const std::string& applicationName, const std::string& command);
        p(std::string, data);
        p(std::string, errorData);
        p(int, result);
        void Execute();
    };

    std::string const Dump(Process const&target);
}