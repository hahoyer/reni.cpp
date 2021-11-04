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

    p(std::string, FullName);
    p_mutable(std::string, Name);
    p_mutable(std::string, Data);
    p(bool, IsValid);
    p_mutable(bool, IsValidFolder);
  };
}
