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

        p(Array<T>, ToArray);

        class Iterator
        {
        public:
            virtual_p(bool, IsValid) = 0;
            virtual Iterator& operator++(int) = 0;
            virtual T const operator*()const = 0;
        };

        class StandardIterator
        {
            Var<Iterator> _data;
        public:
            StandardIterator(Var<Iterator> data) 
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
            function<Var<Iterator>()> _iterator;
        public:
            Container(function<Var<Iterator>()> iterator) : _iterator(iterator){}
            mutable_p_function(Var<Iterator>, ToIterator) const override{ return _iterator(); }
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

        Var<Iterator> SkipIterator(int count)const
        {
            auto result = ToIterator;
            while (count > 0 && result->IsValid)
            {
                (*result)++;
                count--;
            }
            return result;
        }

        class TakeIterator final : public Iterator
        {
            Var<Iterator> _parent;
            int _count;
        public:
            TakeIterator(Var<Iterator> parent, int count)
                : _parent(parent)
                , _count(count)
            {
            }
        protected:
            p_function(bool, IsValid) override{ return _count > 0 && _parent->IsValid; }
            Iterator& operator++(int) override{ --_count; (*_parent)++; return *this; }
            T const operator*()const override{ return **_parent; }
        };

        class PlusIterator final : public Iterator
        {
            Var<Iterator> _left;
            Var<Iterator> _right;
        public:
            PlusIterator(Var<Iterator> left, Var<Iterator> right)
                : _left(left)
                , _right(right)
            {
            }
        private:
            p_function(bool, IsValid) override{ return _left->IsValid || _right->IsValid; }

            Iterator& operator++(int) override
            {
                if (_left->IsValid)
                    (*_left)++;
                else
                    (*_right)++;
                return *this;
            }

            T const operator*()const override
            {
                if (_left->IsValid)
                    return **_left;
                return **_right;
            }
        };

        class WhereIterator final : public Iterator
        {
            Var<Iterator> _parent;
            function<bool(T)> _selector;
        public:
            WhereIterator(Var<Iterator> parent, function<bool(T)> selector)
                : _parent(parent)
                , _selector(selector)
            {
                Align();
            }

            void Align()
            {
                while (_parent->IsValid && !selector(**_parent))
                    (*_parent)++;
            }
        protected:
            p_function(bool, IsValid) override{ return _parent->IsValid; }
            Iterator& operator++(int) override{ (*_parent)++; Align(); return *this; }
            T const operator*()const override{ return **_parent; }
        };

    public:
        Ref<thisType> const Skip(int count) const
        {
            return Ref<thisType>(new Container([=](){return SkipIterator(count); }));
        }

        Ref<thisType> const Take(int count) const
        {
            return Ref<thisType>(new Container([=](){return Var<Iterator>(*new TakeIterator(ToIterator, count)); }));
        }

        Ref<thisType> const operator+(thisType const& right)const
        {
            auto leftIterator = ToIterator;
            auto rightIterator = right.ToIterator;
            return Ref<thisType>(new Container([=](){return Var<Iterator>(*new PlusIterator(leftIterator, rightIterator)); }));
        }

        Ref<thisType> const Where(function<bool(T)> selector)const
        {
            return Ref<thisType>(new Container([=](){return Var<Iterator>(*new WhereIterator(ToIterator, selector)); }));
        }

        T const Stringify(T const&delimiter)const;

        StandardIterator const begin()const{ return ToIterator; }
        StandardIterator const end()const{ return StandardIterator(nullptr); }
    };

}

#include "Array.h"
#include <vector>

using namespace HWLib;

template<typename T>
p_implementation(Enumerable<T>, Array<T>, ToArray)
{
    auto result = std::vector<T>();
    auto i = ToIterator;
    for (; i->IsValid; (*i)++)
        result.push_back(**i);
    return Array<T>(result.size(), [=](int i){return result[i]; });

}

template<typename T>
T const Enumerable<T>::Stringify(T const&delimiter)const
{
    auto result = T();
    auto useDelimiter = false;
    for (auto element : *this)
    {
        if (useDelimiter)
            result = result + delimiter;
        useDelimiter = true;
        result = result + element;
    }
    return result;
}

//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")
