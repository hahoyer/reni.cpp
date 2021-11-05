#pragma once
#include <functional>
using std::function;

namespace HWLib
{
  template <class T>
  class RuntimeChain final
  {
    typedef typename T::dataType DataType;

    class Iterator final : public Enumerable<DataType>::Iterator
    {
      const RuntimeChain<T>* current;
    public:
      Iterator()
        : current(begin) { }

    protected:
      p_function(bool, IsValid) override
      {
        return !!current;
      };

      DataType Step() override;
    };

    static const RuntimeChain* begin;
    const RuntimeChain* const next;
  public:
    DataType data;
    RuntimeChain(DataType data);

    static CtrlRef<Enumerable<DataType>> All()
    {
      return new typename Enumerable<DataType>::Container(new Iterator);
    };
  };

  template <class T>
  const RuntimeChain<T>* RuntimeChain<T>::begin = {};

  template <class T>
  RuntimeChain<T>::RuntimeChain(DataType data)
    : next(begin)
      , data(data)
  {
    begin = this;
  }

  template <class T>
  typename T::dataType RuntimeChain<T>::Iterator::Step()
  {
    auto result = current->data;
    current = current->next;
    return result;
  }
}
