#pragma once

#include "BreakHandling.h"
#include "DefaultAssignmentOperator.h"
#include "String.h"
#include "Common.h"
#include <memory>
using std::shared_ptr;

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
        explicit Ref(T const& value) :_value(new T(value)){}
        Ref(Ref<T> const&value) :_value(value._value){}
        DefaultAssignmentOperator;

        T const& operator*()const { return _value.operator*(); };
        T const* operator->()const { return _value.operator->(); };
        T & operator*(){ return _value.operator*(); };
        T * operator->(){ return _value.operator->(); };
    };

    template<typename T>
    class OptRef : public Ref<T>
    {
        using thisType = OptRef<T>;
        using baseType = Ref<T>;
    public:
        OptRef() = default;
        OptRef(T *value) :baseType(value){}
        explicit OptRef(T const&value) :baseType(new T(value)){}
        OptRef(Ref<T> &value) :baseType(value){}
        OptRef(OptRef<T> const&value) :baseType(value){}
        p(bool, IsValid){ return !!_value.get(); }
        DefaultAssignmentOperator;

        friend OptRef<T> operator||(OptRef<T> left, function<T*()> right)
        {
            return left.IsValid ? left : right();
        }
    };

    template<typename T>
    class Value{};

    template<typename T>
    class Optional
    {
        using thisType = Optional;

        T const _value;
    public:
        Optional() : _value(Value<T>::NotValid){}
        Optional(decltype(null)) : _value(Value<T>::NotValid){}
        Optional(T const value) :_value(value){}

        p(bool, IsValid){ return _value != Value<T>::NotValid; }
        
        DefaultAssignmentOperator;

        operator T const ()const
        {
            assert(IsValid);
            return _value;
        };

        friend Optional<T> operator||(Optional<T> left, function<T()> right)
        {
            return left.IsValid ? left: right();
        }
    };

    template<>
    class Value<int>
    {
    public:
        static int const NotValid = 0x80000000;
    };

}

