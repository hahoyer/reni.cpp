#pragma once

#include <functional>
using std::function;

#include "DefaultAssignmentOperator.h"
#include "Properties.h"
#include "Ref.h"

namespace HWLib
{
    template<typename T> class Array;

    template<typename T>
    class Enumerable
    {
        using thisType = Enumerable<T>;

    public:
        class WithCount
        {
        public:
            virtual_p(int, Count) = 0;
        };

        int const Count(function<bool(T)> selector = [](T){return true; })const;
        T const First(function<bool(T)> selector = [](T){return true; })const;
        T const Single(function<bool(T)> selector = [](T){return true; })const;
        OptRef<T> const FirstOrDefault(function<bool(T)> selector = [](T){return true; })const;
        OptRef<T> const SingleOrDefault(function<bool(T)> selector = [](T){return true; })const;
        T const Stringify(T const&delimiter)const;

        template<typename TResult>
        Ref<Enumerable<TResult>> const Select(function<TResult(T)> selector) const;
        template<typename TResult>
        Ref<Enumerable<TResult>> const SelectMany(function<TResult(T)> selector) const;

        p(Array<T>, ToArray);

        class Iterator
        {
        public:
            virtual_p(bool, IsValid) = 0;
            virtual Iterator& operator++(int) = 0;
            virtual T const operator*()const = 0;
        };

    protected:
        p_definition(Var<Iterator>, ToIterator);
        virtual mutable_p_function(Var<Iterator>, ToIterator)const = 0;

    private:
        template <typename TLeft>
        OptRef<T> const getPlus(TLeft const&parent, thisType const& other, int index)
        {
            auto result = parent.get(index);
            if (result.IsValid)
                return result;
            return other.get(index - parent.Count);
        }

        class Container: public Enumerable<T>
        {
            function<Var<Iterator>()> _iterator;
            mutable_p_function(Var<Iterator>, ToIterator) const override{ return *_iterator(); }
        public:
            Container(function<Var<Iterator>()> iterator) : _iterator(iterator){}
        };

        class SkipIterator final: public Iterator
        {
            Var<Iterator> _parent;
            int const _count;
        public:
            SkipIterator(Iterator& parent, int count) : _parent(parent), _count(count){}
        private:
            p_function(bool, IsValid) override;
            Iterator& operator++(int) override;
            T const operator*()const override;

        };

    public:
        Ref<thisType> const Skip(int count) const
        {
            return *new Container([=](){return new SkipIterator(*ToIterator,count); });
        }

        Ref<thisType> const Take(int count) const;
            /*
        Ref<thisType> const Take(int count) const
        {
            return *new Container(*this, [=](Enumerable<T> const&parent, int index){return index < count ? parent.get(index) : null(); });
        }

        template <typename TLeft>
        friend Ref<thisType> const operator+(TLeft const& left, thisType const& other)
        {
            return *new Container(*this, [=](TLeft const&parent, int index){return getPlus(parent, other, index); });
        }

        Ref<thisType> const Where(function<bool(T)> selector)const
        {
            return *new WhereContainer(*this, selector);
        } */
    };

}

#include "Array.h"
#include <vector>

using namespace HWLib;

template<typename T>
p_implementation(Enumerable<T>, Array<T>, ToArray)
{
    auto result = std::vector<T>();
    Iterator* i = ToIterator.get();
    for (; i.IsValid; (i)++)
        result.push_back(*i);
    return Array<T>(result.size(), [=](int i){return result[i]; });

}

