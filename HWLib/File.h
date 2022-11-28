#pragma once
#include "Properties.h"
#include "CtrlRef.h"

namespace HWLib
{
  class File final
  {
    class internal;
    CtrlRef<internal> _internal;
  public:
    File(const std::string& name);

    HW_PR_GET(std::string, FullName);
    HW_PR_VAR(std::string, Name);
    HW_PR_VAR(std::string, Data);
    HW_PR_GET(bool, IsValid);
    HW_PR_VAR(bool, IsValidFolder);
  };
}
