#pragma once

#include "DefaultOperators.h"
#include "Properties.h"
#include <functional>
#include <optional>

namespace HWLib
{
  template <class T>
  class Optional
  {
    using thisType = Optional;
    using targetType = typename remove_const<T>::type;
    std::optional<targetType> data;

  public:
    Optional() { Initialize(); };
    Optional(T const& data) : data(data) { Initialize(); };
    Optional(thisType const& other) : data(other.data) { Initialize(); };
    HW_DO_PLACEMENT_ASSIGN;
    p(bool, IsValid) { return data.has_value(); };
    p(bool, IsEmpty) { return !data.has_value(); };

    p_definition(T, Value);

    T p_name(Value)() const
    {
      HW_ASSERT_(IsValid);
      return *data;
    };

    T p_name(Value)()
    {
      HW_ASSERT_(IsValid);
      return *data;
    };

    operator T const() const { return Value; };

    thisType operator||(Optional<T> right) const
    {
      if(IsValid)
        return *this;
      return right;
    }

    T operator||(T right) const
    {
      if(IsValid)
        return Value;
      return right;
    }

    thisType operator||(function<Optional<T>()> right) const
    {
      if(IsValid)
        return *this;
      return right();
    }

    bool operator==(Optional<T> const& other) const
    {
      if(IsEmpty)
        return other.IsEmpty;
      return Value == other.Value;
    };

  private:
    static void Initialize()
    { };
  };

  template <typename T>
  std::string Dump(Optional<T> const& target)
  {
    if(target.IsValid)
      return "?{ " + HWLib::Dump(target.Value) + " }";
    return "?{}";
  }
}
