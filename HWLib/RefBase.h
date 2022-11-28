#pragma once

#include "DefaultOperators.h"

namespace HWLib
{
  template <class T, class TDataContainer>
  class RefBase
  {
    using thisType = RefBase;

  public:
    typedef TDataContainer dataContainerType;

  protected:
    dataContainerType value;

    RefBase() { }
    RefBase(T& value) : RefBase(&value) {}
    RefBase(T* value) : value(value) {};

    template <typename TOther, class TOtherData>
    RefBase(RefBase<TOther, TOtherData>& other) : RefBase(other.operator*()) {};
    RefBase(dataContainerType const& value) : value(value) {}

  public:
    HW_DO_PLACEMENT_ASSIGN;
    T const& operator*() const { return *value; };
    T const* operator->() const { return &*value; };
    T& operator*() { return *value; };
    T* operator->() { return &*value; };
    bool const operator==(thisType const& other) const { return value == other.value; };
  };
}
