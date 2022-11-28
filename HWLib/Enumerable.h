#pragma once
//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")

#include "CtrlRef.h"
#include "Properties.h"

using std::function;

namespace HWLib
{
  template <typename T>
  class Array;
  template <typename T>
  class Optional;

  template <typename T>
  class Enumerable
  {
    typedef Enumerable thisType;

  public:
    Enumerable() = default;
    Enumerable(const thisType&) = delete;

    virtual ~Enumerable() = default;

    typedef T targetType;
    template <typename TResult>
    using AggregateFunction = function<const TResult (TResult, T)>;

    template <typename TResult>
    TResult Aggregate(TResult start, AggregateFunction<TResult> selector) const;

    template <typename TResult>
    const TResult Aggregate(AggregateFunction<TResult> selector) const { return Aggregate(TResult(), selector); };

    template <typename TResult>
    const TResult Aggregate() const
    {
      return Aggregate(AggregateFunction<TResult>([&](TResult current, T next) { return current + next; }));
    };
    template <typename TResult>
    CtrlRef<Enumerable<TResult>> Convert() const;
    template <typename TResult>
    CtrlRef<Enumerable<TResult>> ConvertMany() const;
    Optional<size_t> FirstIndex(function<bool(T)> selector) const;
    Optional<T> Max() const;
    const Optional<T> Max(function<bool(T)> selector) const;
    const Optional<T> Max(function<bool(T, T)> isLess) const;
    CtrlRef<thisType> Merge(const thisType& right, function<bool(T, T)> isLess, bool removeDuplicates) const;
    CtrlRef<thisType> operator+(const thisType& right) const;
    template <typename TOther>
    CtrlRef<Enumerable<std::pair<T, TOther>>> operator*(const Enumerable<TOther>& other) const;
    template <typename TResult>
    CtrlRef<Enumerable<TResult>> Select(function<TResult(T)> selector) const;
    CtrlRef<thisType> Skip(size_t count) const;
    CtrlRef<Enumerable> Sort(function<bool(T, T)> isLeftSmaller) const;
    T Stringify(const T& delimiter) const;
    T Sum() const;
    CtrlRef<thisType> Take(size_t count) const;
    CtrlRef<thisType> Where(function<bool(T)> selector) const;

    HW_PR_GET(bool, Any);
    HW_PR_GET(size_t, Count);
    HW_PR_GET(T, First);
    HW_PR_GET(Optional<T>, FirstOrDefault);
    HW_PR_GET(T, Last);
    HW_PR_GET(Optional<T>, LastOrDefault);
    HW_PR_GET(T, Single);
    HW_PR_GET(Optional<T>, SingleOrDefault);
    HW_PR_GET(Array<T>, ToArray);

    class Iterator // It's a one-time-access-forward-read-only iterator
    {
    public:
      virtual ~Iterator() {};
      HW_PR_VIRTUAL_GET_INLINE(bool, IsValid) = 0;

    public:
      virtual T Step() = 0;
      HW_PR_MUTABLE_GET(Array<T>, ToArray);
    };

    class RangeBasedForLoopSimulator;
    class Container;

    HW_PR_VIRTUAL_GET_INLINE(CtrlRef<Iterator>, ToIterator) = 0;

  public:
    RangeBasedForLoopSimulator begin() const { return ToIterator; }
    RangeBasedForLoopSimulator end() const;

    class EmptySequenceException : public std::exception
    {};

  private:
    template <typename TLeft>
    const Optional<T> getPlus(const TLeft& parent, const thisType& other, int index)
    {
      auto result = parent.get(index);
      if(result.IsValid)
        return result;
      return other.get(index - parent.Count);
    }
  };


  CtrlRef<Enumerable<size_t>> Numbers(size_t count);
}

//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")
