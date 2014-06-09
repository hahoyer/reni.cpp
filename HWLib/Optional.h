#pragma once

#include "DefaultAssignmentOperator.h"
using namespace std;

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
        Optional(T const value) :value(value){}
        template<class TOther>
        Optional(Optional<TOther> const value) 
            : value(value.IsEmpty ? Constants<T>::NotValid: value.Value  ){}

        p(bool, IsValid){ return value != Constants<T>::NotValid; };
        p(bool, IsEmpty){ return value == Constants<T>::NotValid; };

        DefaultAssignmentOperator;

        p(T, Value){
            a_if_(IsValid);
            return value;
        };

        operator T const ()const{ return Value; };

        friend Optional<T> operator||(Optional<T> left, Optional<T> right)
        {
            if(left.IsValid)
                return left;
            return right;
        }

        friend Optional<T> operator||(Optional<T> left, T right)
        {
            if(left.IsValid)
                return left;
            return right;
        }

        friend Optional<T> operator||(Optional<T> left, function<Optional<T>()> right)
        {
            if(left.IsValid)
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
        Optional(bool const value) :value(value?-1:0){}

        p(bool, IsValid){return value < 1;}
        p(bool, IsEmpty){ return !IsValid; }

        DefaultAssignmentOperator;

        p(bool, Value){
            a_if_(IsValid);
            return value < 0;
        };

        operator bool const ()const{ return Value; };

        friend Optional<bool> operator||(Optional<bool> left, function<Optional<bool>()> right){
            if(left.IsValid)
                return left;
            return right();
        }
    };

    template<typename T>
    inline bool const operator==(Optional<T> const&left, Optional<T> const&other)
    {
        if(left.IsEmpty)
            return other.IsEmpty;
        return left.Value == other.Value;
    };

    class String; 
    
    template <typename T>
    String const Dump(Optional<T> const&target)
    {
        if(target.IsValid)
            return "?{ " + HWLib::Dump(target.Value) + " }";
        return "?{}";
    }
}

