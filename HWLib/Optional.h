#pragma once

#include "DefaultAssignmentOperator.h"

namespace HWLib
{
    template<typename T>
    class Constants final{};

    template<>
    class Constants <int>{
    public:
        static int const NotValid = 0x80000000;
    };

    template<typename T>
    class Optional final{
        using thisType = Optional;

        T const value;
    public:
        Optional() : value(Constants<T>::NotValid){}
        Optional(decltype(null)) : value(Constants<T>::NotValid){}
        Optional(T const value) :value(value){}

        p(bool, IsValid){ return value != Constants<T>::NotValid; }
        
        DefaultAssignmentOperator;

        p(T, Value){
            a_if_(IsValid);
            return value;
        };

        operator T const ()const{ return Value; };

        friend Optional<T> operator||(Optional<T> left, std::function<Optional<T>()> right){
            if (left.IsValid)
                return left;
            return right();
        }
    };

}

