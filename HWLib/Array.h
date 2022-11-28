#pragma once
//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")

#include "_.h"
#include "Common.h"
#include "Enumerable.h"
#include <vector>
#include <queue>

namespace HWLib
{
  template <typename T>
  class Array final : public Enumerable<T>
  {
    using baseType = Enumerable<T>;
    using thisType = Array<T>;

    const size_t _count;
    T* const _data;

  public:
    Array() : _count(0), _data(nullptr) { }

    Array(Array<T>&& other)
      : _count(other.Count)
        , _data(other._data)
    {
      const_cast<const T*&>(other._data) = nullptr;
      const_cast<size_t&>(other._count) = 0;
    };

    Array(const Array<T>& other)
      : _count(other.Count)
        , _data(reinterpret_cast<T* const>(new __int8[sizeof(T) * other.Count]))
    {
      auto data = const_cast<typename remove_const<T>::type*>(_data);
      for(auto index = 0; index < Count; index++)
        new(data + index) T(other[index]);
    };

    Array(std::queue<std::unique_ptr<T>>& other)
      : _count(other.size())
        , _data(reinterpret_cast<T* const>(new __int8[sizeof(T) * other.size()]))
    {
      auto data = const_cast<typename remove_const<T>::type*>(_data);
      for(auto index = 0; index < _count; index++)
      {
        new(data + index) T(*other.front());
        other.pop();
      }
    }

    explicit Array(const std::vector<T>& other)
      : _count(other.size())
        , _data(reinterpret_cast<T* const>(new __int8[sizeof(T) * other.size()]))
    {
      auto data = const_cast<typename remove_const<T>::type*>(_data);
      auto index = 0;
      for(const auto& element : other)
      {
        new(data + index) T(element);
        index++;
      }
    }

    Array(const std::initializer_list<T>& other)
      : _count(other.size())
        , _data(reinterpret_cast<T* const>(new __int8[sizeof(T) * other.size()]))
    {
      auto data = const_cast<typename remove_const<T>::type*>(_data);
      auto index = 0;
      for(auto element : other)
      {
        new(data + index) T(element);
        index++;
      }
    }

    virtual ~Array() override
    {
      for(auto i = 0; i < _count; i++)
        _data[i].~T();
      if(_data)
        delete[] reinterpret_cast<const __int8*>(_data);
    }

    HW_DO_PLACEMENT_ASSIGN;

    p(size_t, Count) { return _count; }
    p(T const*, RawData) { return _data; }

    const T& operator[](size_t Index) const { return _data[Index]; }
    T& operator[](size_t Index) { return _data[Index]; }
    thisType operator+(const thisType& other) const { return baseType::operator+(other)->ToArray; }

    thisType operator+(const T& other) const
    {
      auto otherBox = {other};
      return *this + otherBox;
    }

    void operator+=(const T& other) { *this = *this + other; }
    void operator+=(const thisType& other) { *this = *this + other; }

    bool Compare(const Array<T>& other) const;

  private:
    class LocalIterator final : public baseType::Iterator
    {
      using baseType = typename baseType::Iterator;
      using thisType = LocalIterator;
      const Array<T>& _parent;
      size_t _index;

    public:
      LocalIterator(const Array<T>& parent)
        : _parent(parent)
          , _index(0)
      { }

      p_function(bool, IsValid) override { return _index >= 0 && _index < _parent.Count; }
      T Step() override { return _parent[_index++]; }

      void operator=(const LocalIterator&) = delete;
    };

    p_nonconst_function(CtrlRef<typename Enumerable<T>::Iterator>, ToIterator) const override
    {
      return new LocalIterator(*this);
    }
  };

  template <typename T>
  bool Array<T>::Compare(const Array<T>& other) const
  {
    if(Count != other.Count)
      return false;
    for(auto index = 0; index < Count; index++)
      if((*this)[index] != other[index])
        return false;
    return true;
  };


  template <typename T>
  Array<T> _(const std::initializer_list<T>& data) { return data; };
}

//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")
