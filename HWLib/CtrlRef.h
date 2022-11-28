#pragma once

#include <memory>

#include "DefaultOperators.h"
#include "RefBase.h"

namespace HWLib
{
  template <typename T>
  class CtrlRef final : public RefBase<T, std::shared_ptr<T>>
  {
    typedef RefBase<T, std::shared_ptr<T>> baseType;
    typedef CtrlRef thisType;
    friend class CtrlRef<T>;
    friend class CtrlRef<const T>;

  public:
    CtrlRef(T* value) : baseType(value)
    {
      HW_ASSERT_(value);
    }

    CtrlRef(const CtrlRef<T>& other) : baseType(other.value) { };
    CtrlRef(const CtrlRef<const T>& other);

    template <typename TOther>
    CtrlRef(const CtrlRef<TOther>& other) : baseType(other) { };

    HW_DO_PLACEMENT_ASSIGN;
  };

  template <typename T>
  class CtrlRef<const T>final : public RefBase<const T, std::shared_ptr<const T>>
  {
    typedef RefBase<const T, std::shared_ptr<const T>> baseType;
    typedef CtrlRef<const T> thisType;

  public:
    CtrlRef(const T* value) : baseType(value)
    {
      HW_ASSERT_(value);
    }

    CtrlRef(const CtrlRef<const T>& other) : baseType(other) { };

    HW_DO_PLACEMENT_ASSIGN;
    T const& operator*() const { return *baseType::value; };
    T const* operator->() const { return &*baseType::value; };

  private:
    T& operator*();
    T* operator->();
  };

  template <typename T>
  CtrlRef<T>::CtrlRef(const CtrlRef<const T>& other)
    : baseType(other.value) { };
}
