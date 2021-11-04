#pragma once
#include "../HWLib/DumpableObject.h"
#include "../Util/Size.h"

using namespace HWLib;
using namespace Util;

namespace Reni
{
  class Address final : public DumpableObject
  {
    using baseType = DumpableObject;
    using thisType = Address;
  public:
    const Size data;
    const int depth;

    Address(const Size& data, int depth);

    Address operator+(int delta) const;
  private:
    p_function(Array<string>, DumpData) override { return {nd(data), nd(depth)}; };
  };
};
