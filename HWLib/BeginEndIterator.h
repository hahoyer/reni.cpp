#pragma once

namespace HWLib
{
  template <typename T, class TContainer>
  class BeginEndIterator final : public Enumerable<T>::Iterator
  {
    typedef typename Enumerable<T>::Iterator baseType;
    typedef BeginEndIterator thisType;
    typename TContainer::const_iterator iterator;
    TContainer const& container;

  public:
    BeginEndIterator(TContainer const& container)
      : iterator(container.begin())
        , container(container) { }

  private:
    HW_PR_DECL_GETTER(bool, IsValid) override
    {
      return iterator != container.end();
    }

    T Step() override
    {
      return *iterator++;
    }
  };


  template <typename T, typename TContainer>
  class Enumerable_ final : public Enumerable<T>
  {
    typedef Enumerable<T> baseType;
    typedef Enumerable_ thisType;
    TContainer const& container;

  public:
    Enumerable_(TContainer const& container) : container(container) { };

  private:
    HW_PR_DECL_MUTABLE_GETTER(CtrlRef<typename baseType::Iterator>, ToIterator) const override
    {
      return new BeginEndIterator<T, TContainer>(container);
    }
  };

  template <typename TContainer>
  template <typename T>
  Enumerable_<T, TContainer> const Box_<TContainer>::ToEnumerable() const
  {
    return _data;
  }
}
