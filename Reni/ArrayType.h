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

    HW_PR_THISREF;
  private:
    HW_PR_DECL_GETTER(Size, size) override;
    HW_PR_DECL_GETTER(WeakRef<Global>, global) override { return elementType.global; };
    HW_PR_DECL_GETTER(Array<string>, DumpData) override
    {
      return {
        HW_D_VALUE(elementType),
        HW_D_VALUE(count)
      };
    }

    HW_PR_DECL_GETTER(bool, hollow) { return false; };
  };
};
