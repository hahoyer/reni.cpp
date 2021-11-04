#pragma once
#include "Type.h"
#include "../HWLib/WeakRef.h"
#include "../Util/Size.h"

using namespace HWLib;

namespace Reni
{
  class DumpPrintToken;

  class ArrayType final : public Type
  {
    using baseType = Type;
    using thisType = ArrayType;
  public:
    const Type& elementType;
    const size_t count;

    ArrayType(const Type& elementType, size_t count)
      : elementType(elementType)
        , count(count)
    {
      SetDumpString();
    }

    ThisRef;
  private:
    p_function(Size, size) override;
    p_function(WeakRef<Global>, global) override { return elementType.global; };
    p_function(Array<string>, DumpData) override
    {
      return {
        nd(elementType),
        nd(count)
      };
    }

    p_function(bool, hollow) { return false; };
  };
};
