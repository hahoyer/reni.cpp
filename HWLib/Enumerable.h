#pragma once
//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")

#include "DefaultAssignmentOperator.h"
#include "Properties.h"
#include "Ref.h"
#include "BreakHandling.h"

namespace HWLib
{
    template<typename T> class Array;

    template<typename T>
    class Enumerable
    {
        using thisType = Enumerable<T>;

    public:
        int const Count(function<bool(T)> selector = [](T){return true; })const;
        T const First(function<bool(T)> selector = [](T){return true; })const;
        T const Single(function<bool(T)> selector = [](T){return true; })const;
        OptRef<T> const FirstOrDefault(function<bool(T)> selector = [](T){return true; })const;
        OptRef<T> const SingleOrDefault(function<bool(T)> selector = [](T){return true; })const;

        template<typename TResult>
        Ref<Enumerable<TResult>> const Select(function<TResult(T)> selector) const;
        template<typename TResult>
        Ref<Enumerable<TResult>> const SelectMany(function<TResult(T)> selector) const;
        template<typename TSplitter>
        Ref<Enumerable<Ref<Enumerable<T>>>> const Split() const;

        p(Array<T>, ToArray);

        class Iterator
        {
        public:
            virtual_p(bool, IsValid) = 0;
            virtual void operator++(int) = 0;
            virtual T const operator*()const = 0;
            virtual_p(Ref<Iterator>, Clone) = 0;
        };

        class StandardIterator
        {
            Ref<Iterator> _data;
        public:
            StandardIterator(Ref<Iterator> data)
                : _data(data)
            {
            }

            StandardIterator& operator++() { (*_data)++; return *this; };
            T const operator *()const { return **_data; }
            bool operator !=(StandardIterator other)
            {
                assert(other._data.get() == nullptr);
                return _data->IsValid;
            }
        };

        class Container : public Enumerable<T>
        {
            Ref<Iterator> _iterator;
        public:
            Container(Iterator* iterator) : _iterator(iterator){}
            mutable_p_function(Ref<Iterator>, ToIterator) const override{ return _iterator; }
        };

        p_definition(Ref<Iterator>, ToIterator);
        virtual mutable_p_function(Ref<Iterator>, ToIterator)const = 0;

    private:
        template <typename TLeft>
        OptRef<T> const getPlus(TLeft const&parent, thisType const& other, int index)
        {
            auto result = parent.get(index);
            if (result.IsValid)
                return result;
            return other.get(index - parent.Count);
        }

        Ref<Iterator> SkipIterator(int count)const
        {
            auto result = ToIterator;
            while (count > 0 && result->IsValid)
            {
                (*result)++;
                count--;
            }
            return result;
        }

        class TakeIterator;
        class PlusIterator;
        class WhereIterator;

    public:
        Ref<thisType> const Skip(int count) const;
        Ref<thisType> const Take(int count) const;
        Ref<thisType> const operator+(thisType const& right)const;
        Ref<thisType> const Where(function<bool(T)> selector)const;
        T const Stringify(T const&delimiter)const;

        StandardIterator const begin()const{ return ToIterator; }
        StandardIterator const end()const{ return StandardIterator(); }
    };

}

//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")
