#pragma once
//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")

#include "DefaultAssignmentOperator.h"
#include "Properties.h"
#include "BreakHandling.h"

namespace HWLib
{
    template<typename T> class Array;
    template<typename T> class OptRef;
    template<typename T> class Ref;

    template<typename T>
    class Enumerable
    {
        using thisType = Enumerable<T>;

    public:
        template<typename TResult>
        TResult            const Aggregate(function<TResult(TResult, T)> selector)const{ return Aggregate(TResult(), selector); }
        template<typename TResult>
        TResult             const Aggregate     (TResult start, function<TResult(TResult, T)> selector)const;
        T                    const First         (function<bool(T)> selector = [](T){return true; })const;
        OptRef<T>             const FirstOrDefault(function<bool(T)> selector = [](T){return true; })const;
        int                    const Count       (function<bool(T)> selector = [](T){return true; })const;
        Ref<thisType>           const operator+ (thisType const& right)const;
        template<typename TResult>
        Ref<Enumerable<TResult>> const Select    (function<TResult(T)> selector) const;
        template<typename TResult>
        Ref<Enumerable<TResult>> const SelectMany (function<TResult(T)> selector) const;
        T                       const Single       (function<bool(T)> selector = [](T){return true; })const;
        OptRef<T>              const SingleOrDefault(function<bool(T)> selector = [](T){return true; })const;
        Ref<thisType>         const Skip           (int count) const;
        T                   const Stringify      (T const&delimiter)const;
        Ref<thisType>      const Take           (int count) const;
        Ref<thisType>     const Where          (function<bool(T)> selector)const;

        p(Array<T>, ToArray);

        class Iterator
        {
        public:
            virtual_p(bool, IsValid) = 0;
            virtual void operator++(int) = 0;
            virtual T const operator*()const = 0;
            virtual_p(Ref<Iterator>, Clone) = 0;
        };

        class EndPosition final : public Iterator
        {
        public:
            static EndPosition Instance;

            p_function(bool, IsValid) override{ return false; }
            p_function(Ref<Iterator>, Clone) override{ return &Instance; }
            void operator++(int) override{}
            T const operator*()const override{ throw *this; }
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
                assert(&*_data == &EndPosition::Instance);
                return _data->IsValid;
            }

        };

        class Container final : public Enumerable<T>
        {
            Ref<Iterator> _iterator;
        public:
            Container(Iterator* iterator) : _iterator(iterator){}
            mutable_p_function(Ref<Iterator>, ToIterator) const override{ return _iterator; }
        };

        p_definition(Ref<Iterator>, ToIterator);
        virtual mutable_p_function(Ref<Iterator>, ToIterator)const = 0;

        StandardIterator const begin()const{ return ToIterator; }
        StandardIterator const end()const;

    private:
        template <typename TLeft>
        OptRef<T> const getPlus(TLeft const&parent, thisType const& other, int index)
        {
            auto result = parent.get(index);
            if (result.IsValid)
                return result;
            return other.get(index - parent.Count);
        }

        class TakeIterator;
        class PlusIterator;
        class WhereIterator;
        class SkipIterator;
    };

}

//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")
