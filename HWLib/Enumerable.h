#pragma once

#include <functional>
using std::function;

#include "DefaultAssignmentOperator.h"
#include "Properties.h"

namespace HWLib
{
    template<typename T> class Array;

    template<typename T> class IEnumerable
    {
        typedef IEnumerable<T> thisType;
    public:
        class Position
        {
        public:
            Position& operator++();
            T operator*();
            bool const operator!=(Position const&other)const;
        };

        int Count(function<bool(T)> selector)const;
        int Count()const{ return Count([](T){return true; }); }

        thisType const operator+(thisType const& other)const;
        T Stringify(T const& delimiter)const;

        p(Array<T>, ToArray);
        p(int, Length){ return Count(); }

        virtual Position  begin()const = 0;
        virtual Position const end()const = 0;
    };

}


