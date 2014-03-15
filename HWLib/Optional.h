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

        p(bool, IsValid){
            return value != Constants<T>::NotValid;
        }
        
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

    template<>
    class Optional<bool> final{
        using thisType = Optional;

        char const value;
    public:
        Optional() : value(1){}
        Optional(decltype(null)) : value(1){}
        Optional(bool const value) :value(value?-1:0){}

        p(bool, IsValid){
            return value < 1;
        }

        DefaultAssignmentOperator;

        p(bool, Value){
            a_if_(IsValid);
            return value < 0;
        };

        operator bool const ()const{ return Value; };

        friend Optional<bool> operator||(Optional<bool> left, std::function<Optional<bool>()> right){
            if(left.IsValid)
                return left;
            return right();
        }
    };
}

