#pragma once
//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")

#include "DefaultAssignmentOperator.h"
#include "Properties.h"
#include "CtrlRef.h"

using std::function;

namespace HWLib
{
    template<typename T> class Array;
    template<typename T> class Optional;

    template<typename T>
    class Enumerable
    {
        typedef Enumerable thisType;
    public:
        Enumerable() = default;
        Enumerable(thisType const&) = delete;

        virtual ~Enumerable() = default;

        typedef T targetType;
        template<typename TResult>
        using AggregateFunction = function<TResult const(TResult, T)>;

        template<typename TResult>
        TResult                        const Aggregate (TResult start, AggregateFunction<TResult> selector)const;
        template<typename TResult>
        TResult                         const Aggregate(AggregateFunction<TResult> selector)const{ return Aggregate(TResult(), selector); };
        template<typename TResult>
        TResult                          const Aggregate() const{ return Aggregate(AggregateFunction<TResult>([&](TResult current, T next){return current + next; })); };
        template<typename TResult>
        CtrlRef<Enumerable<TResult>>      const Convert    () const;
        template<typename TResult>
        CtrlRef<Enumerable<TResult>>       const ConvertMany() const;
        Optional<size_t> const FirstIndex(function<bool(T)> selector) const;
        Optional<T>                          const Max()const;
        Optional<T>                           const Max(function<bool(T)> selector)const;
        Optional<T>                            const Max(function<bool(T, T)> isLess)const;
        CtrlRef<thisType>                       const Merge    (thisType const& right, function<bool(T, T)> isLess, bool removeDuplicates)const;
        CtrlRef<thisType>                        const operator+(thisType const& right)const;
        template<typename TOther>
        CtrlRef<Enumerable<std::pair<T, TOther>>> const operator*(Enumerable<TOther>const&other)const;
        template<typename TResult>
        CtrlRef<Enumerable<TResult>>             const Select   (function<TResult(T)> selector) const;
        CtrlRef<thisType>                       const Skip       (size_t count) const;
        CtrlRef<Enumerable>                    const Sort       (function<bool(T,T)>isLeftSmaller) const;
        T                                     const Stringify  (T const&delimiter)const;
        T                                    const Sum        ()const;
        CtrlRef<thisType>                   const Take       (size_t count) const;
        CtrlRef<thisType>                  const Where      (function<bool(T)> selector)const;

        p(bool,    Any           );
        p(size_t,   Count         );
        p(T,         First         );
        p(Optional<T>, FirstOrDefault);
        p(T, Last);
        p(Optional<T>, LastOrDefault);
        p(T,         Single         );
        p(Optional<T>, SingleOrDefault);
        p(Array<T>,  ToArray        );

        class Iterator // It's a one-time-access-forward-read-only iterator
        {
        public:
            virtual ~Iterator(){};
            p_virtual_inline(bool, IsValid) = 0;
            virtual T const Step() = 0;
            p_nonconst(Array<T> const, ToArray);
        };

        class RangeBasedForLoopSimulator;

        class Container final : public Enumerable<T>
        {
            CtrlRef<Iterator> _iterator;
        public:
            Container(Iterator* iterator) : _iterator(iterator){}
            p_nonconst_function(CtrlRef<Iterator>, ToIterator) const override{ return _iterator; }
        };

        p_virtual_definition(CtrlRef<Iterator>,ToIterator);
        CtrlRef<Iterator> p_virtual_name(ToIterator)()const { return p_name(ToIterator)(); }
        virtual p_nonconst_function(CtrlRef<Iterator>, ToIterator)const = 0;

        RangeBasedForLoopSimulator const begin()const{ return ToIterator; }
        RangeBasedForLoopSimulator const end()const;

        class EmptySequenceException : public std::exception{};
    private:
        template <typename TLeft>
        Optional<T> const getPlus(TLeft const&parent, thisType const& other, int index)
        {
            auto result = parent.get(index);
            if (result.IsValid)
                return result;
            return other.get(index - parent.Count);
        }

    };


    CtrlRef<Enumerable<size_t>> const Numbers(size_t count);

}

//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")
