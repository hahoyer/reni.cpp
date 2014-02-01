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
        using AggregateFunction = function<TResult const(TResult, T)>;

        template<typename TResult>
        TResult            const Aggregate     (TResult start, AggregateFunction<TResult> selector)const;
        template<typename TResult>
        TResult             const Aggregate     (AggregateFunction<TResult> selector)const{ return Aggregate(TResult(), selector); }
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

        class Iterator // It's a one-time-access-forward-read-only iterator
        {
        public:
            virtual ~Iterator(){};
            virtual_p(bool, IsValid) = 0;
            virtual T const Step() = 0;
            Array<T> const ToArray();
        };

        class RangeBasedForLoopSimulator final
        {
            OptRef<Iterator> _data;
            mutable bool _hasBeenAccessed;

        public:
            RangeBasedForLoopSimulator(Ref<Iterator> data)
                : _data(data)
                , _hasBeenAccessed(false)
            {
            }
            RangeBasedForLoopSimulator(){}

            virtual ~RangeBasedForLoopSimulator(){}

            void operator++() 
            {
                if (_hasBeenAccessed)
                    _hasBeenAccessed = false;
                else
                    _data->Step();
            };
            
            T const operator *()const 
            { 
                assert(!_hasBeenAccessed);
                _hasBeenAccessed = true;
                return const_cast<RangeBasedForLoopSimulator&>(*this)._data->Step();
            }
            
            bool operator !=(RangeBasedForLoopSimulator other)const
            {
                assert(!other._data.IsValid);
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

        RangeBasedForLoopSimulator const begin()const{ return ToIterator; }
        RangeBasedForLoopSimulator const end()const;

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
