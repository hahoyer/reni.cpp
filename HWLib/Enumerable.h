#pragma once
//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")

#include "DefaultAssignmentOperator.h"
#include "Properties.h"
#include "CtrlPtr.h"
#include "CtrlRef.h"

using std::function;

namespace HWLib
{
    template<typename T> class Array;

    template<typename T>
    class Enumerable
    {
        typedef Enumerable thisType;
    public:
        typedef T targetType;
        template<typename TResult>
        using AggregateFunction = function<TResult const(TResult, T)>;

        template<typename TResult>
        TResult                          const Aggregate  (TResult start, AggregateFunction<TResult> selector)const;
        template<typename TResult>
        TResult                           const Aggregate  (AggregateFunction<TResult> selector)const{ return Aggregate(TResult(), selector); }
        template<typename TResult>
        CtrlRef<Enumerable<TResult>>       const Convert    () const;
        template<typename TResult>
        CtrlRef<Enumerable<TResult>>        const ConvertMany() const;
        CtrlPtr<T>                            const Max      ()const;
        CtrlPtr<T>                             const Max      (function<bool(T)> selector)const;
        CtrlPtr<T>                              const Max      (function<bool(T,T)> isLess)const;
        CtrlRef<thisType>                        const operator+(thisType const& right)const;
        template<typename TOther>
        CtrlRef<Enumerable<std::pair<T, TOther>>> const operator*(Enumerable<TOther>const&other)const;
        template<typename TResult>
        CtrlRef<Enumerable<TResult>>             const Select   (function<TResult(T)> selector) const;
        template<typename TResult>
        CtrlRef<Enumerable<TResult>>            const SelectMany (function<TResult(T)> selector) const;
        CtrlRef<thisType>                      const Skip       (int count) const;
        T                                     const Stringify  (T const&delimiter)const;
        CtrlRef<thisType>                    const Take       (int count) const;
        CtrlRef<thisType>                   const Where      (function<bool(T)> selector)const;

        p(int,      Count);
        p(T,         First);
        p(CtrlPtr<T>, FirstOrDefault);
        p(T,         Last);
        p(CtrlPtr<T>, LastOrDefault);
        p(T,         Single);
        p(CtrlPtr<T>, SingleOrDefault);

        p(Array<T>, ToArray);

        class Iterator // It's a one-time-access-forward-read-only iterator
        {
        public:
            virtual ~Iterator(){};
            virtual_p_inline(bool, IsValid) = 0;
            virtual T const Step() = 0;
            mutable_p(Array<T> const, ToArray);
        };

        class RangeBasedForLoopSimulator final
        {
            CtrlPtr<Iterator> _data;
            mutable bool _hasBeenAccessed;

        public:
            RangeBasedForLoopSimulator(CtrlRef<Iterator> data)
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
                a_if_(!_hasBeenAccessed);
                _hasBeenAccessed = true;
                return const_cast<RangeBasedForLoopSimulator&>(*this)._data->Step();
            }
            
            bool operator !=(RangeBasedForLoopSimulator other)const
            {
                a_if_(!other._data.IsValid);
                return _data->IsValid;
            }

        };

        class Container final : public Enumerable<T>
        {
            CtrlRef<Iterator> _iterator;
        public:
            Container(Iterator* iterator) : _iterator(iterator){}
            mutable_p_function(CtrlRef<Iterator>, ToIterator) const override{ return _iterator; }
        };

        p_definition(CtrlRef<Iterator>, ToIterator);
        virtual mutable_p_function(CtrlRef<Iterator>, ToIterator)const = 0;

        RangeBasedForLoopSimulator const begin()const{ return ToIterator; }
        RangeBasedForLoopSimulator const end()const;

    private:
        template <typename TLeft>
        CtrlPtr<T> const getPlus(TLeft const&parent, thisType const& other, int index)
        {
            auto result = parent.get(index);
            if (result.IsValid)
                return result;
            return other.get(index - parent.Count);
        }

    };

}

//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")
