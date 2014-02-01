#pragma once

#include "BreakHandling.h"
#include "String.h"
#include "Common.h"
using boost::shared_ptr;

namespace HWLib
{
    template<typename T>

    class Ref
    {
        using thisType = Ref<T>;
    protected:
        shared_ptr<T> _value;
        Ref() : _value(nullptr) {}
    public:
        Ref(T *value) :_value(value){}
        Ref(T & value) :_value(new T(value)){}
        Ref(Ref<T> const&value) :_value(value._value){}
        virtual ~Ref(){};
        DefaultAssignmentOperator;

        T const& operator*()const { return _value.operator*(); };
        T const* operator->()const { return _value.operator->(); };
        T & operator*(){ return _value.operator*(); };
        T * operator->(){ return _value.operator->(); };
    };

    template<typename T>
    class OptRef final : public Ref<T>
    {
        using thisType = OptRef<T>;
        using baseType = Ref<T>;
    public:
        OptRef() = default;
        OptRef(T *value) :baseType(value){}
        OptRef(T &value) :baseType(value){}
        OptRef(Ref<T> const&other) :baseType(other){}
        OptRef(OptRef<T> const&other) :baseType(other){}
        virtual ~OptRef(){};

        p(bool, IsValid){ return !!_value.get(); }
        DefaultAssignmentOperator;

        friend OptRef<T> operator||(OptRef<T> left, function<T*()> right)
        {
            return left.IsValid ? left : right();
        }
    };

    template<typename T>
    class Constants final{};

    template<>
    class Constants <int>
    {
    public:
        static int const NotValid = 0x80000000;
    };

    template<typename T>
    class Optional final
    {
        using thisType = Optional;

        T const _value;
    public:
        Optional() : _value(Constants<T>::NotValid){}
        Optional(decltype(null)) : _value(Constants<T>::NotValid){}
        Optional(T const value) :_value(value){}

        p(bool, IsValid){ return _value != Constants<T>::NotValid; }
        
        DefaultAssignmentOperator;

        p(T, Value)
        {
            assert(IsValid);
            return _value;
        };

        operator T const ()const{ return Value; };

        friend Optional<T> operator||(Optional<T> left, function<Optional<T>()> right)
        {
            if (left.IsValid)
                return left;
            return right();
        }
    };

}

