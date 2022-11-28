#pragma once
//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")

#include "Common.h"
#include "Properties.h"
#include "DefaultOperators.h"

#undef _

namespace HWLib
{
  class String;

  template <typename T>
  class Box_
  {
    template <typename T, typename TContainer>
    friend class Enumerable_;

    using thisType = Box_<T>;
    T& _data;

  public:
    Box_(T& data) : _data(data) {}
    Box_(const thisType& data) = delete;

    HW_PR_MUTABLE_GET(T&, Data) { return _data; }

    void Delete() const { delete &_data; };

    template <typename TDestination>
    TDestination Convert() { return convert<TDestination>(_data); };
    const String DumpToString() const;
    template <typename TElement>
    const Enumerable_<TElement, T> ToEnumerable() const;
  };

  template <typename TPointer>
  class Box_<TPointer*>
  {
    using thisType = Box_<TPointer*>;
    using T = TPointer*;

    T& _data;

  public:
    Box_(T& data) : _data(data) {}
    Box_(const thisType& data) = delete;

    HW_PR_MUTABLE_GET(T&, Data) { return _data; }

    void SaveDelete() const
    {
      HW_ASSERT_(_data);
      delete _data;
      _data = nullptr;
    };

    void SmartDelete() const
    {
      if(_data) delete _data;
      _data = nullptr;
    };

    void SaveDeleteArray() const
    {
      HW_ASSERT_(_data);
      delete[] _data;
      _data = nullptr;
    };

    void SmartDeleteArray() const
    {
      if(_data) delete[] _data;
      _data = nullptr;
    };
  };

  template <typename TPointer>
  class Box_<TPointer*const>
  {
    HW_DO_DELETE(Box_<TPointer*const>, CC, MC, CA, MA);

    using thisType = Box_<TPointer*const>;
    using T = TPointer*const;

    T& _data;

  public:
    Box_(T& data) : _data(data) {}

    HW_PR_MUTABLE_GET(T&, Data) { return _data; }

    void SaveDelete() const
    {
      HW_ASSERT_(_data);
      delete _data;
    };
    void SmartDelete() const { if(_data) delete _data; };

    void SaveDeleteArray() const
    {
      HW_ASSERT_(_data);
      delete[] _data;
    };
    void SmartDeleteArray() const { if(_data) delete[] _data; };
  };

  template <typename T>
  Box_<T> _(T& data) { return data; }
}

//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")
