#pragma once
#include "Optional.h"

using std::function;

namespace HWLib
{
  template <typename T>
  class ValueCache final
  {
    using thisType = ValueCache;

    mutable Optional<T> value;
    mutable bool isBusy;

  public:
    function<T()> const getValue;

    ValueCache(function<T()> getValue)
      : getValue(getValue)
        , isBusy(false)
    { }

    HW_PR_VAR_SETTER(bool, IsValid) const
    {
      if(value)
        Ensure();
      else
        Reset();
    }

    HW_PR_GET(bool, IsBusy) { return isBusy; }

    HW_PR_MUTABLE_GET(T, Value) const
    {
      Ensure();
      return value.Value;
    }

  private:
    void Ensure() const
    {
      HW_ASSERT_(!isBusy);
      if(value.IsValid)
        return;
      isBusy = true;
      value = getValue();
      isBusy = false;
    }

    void Reset() const
    {
      HW_ASSERT_(!isBusy);
      value = {};
    }
  };

  template <typename T>
  HW_PR_IMPL_GETTER(ValueCache<T>, bool, IsValid) { return value.IsValid; }
}
