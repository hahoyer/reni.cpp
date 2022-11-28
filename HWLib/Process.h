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
        HW_PR_GET(std::string, data);
        HW_PR_GET(std::string, errorData);
        HW_PR_GET(int, result);
        void Execute();
    };

    std::string const Dump(Process const&target);
}