#pragma once

#include "DefaultAssignmentOperator.h"

namespace HWLib
{
    class String; 
    template<typename T>class Array;

    template<typename T>
    class WeakRef
    {
        using thisType = WeakRef;
    public:
        using dataContainerType = T*;
    private:
        dataContainerType _value;
    public:
        WeakRef(T &value) :_value(&value){ }
        WeakRef(T *value) :_value(value){ a_if_(IsValid); }
        WeakRef(WeakRef<T> const&value) : _value(value.value){ };
        template<typename TOther>
        WeakRef(WeakRef<TOther> const&other) : _value(other.value){ };
        DefaultAssignmentOperator;

        bool const operator==(thisType const&other)const{ return _value == other._value; }
        T const& operator*()const { return *_value; };
        T const* operator->()const { return _value; };
        T & operator*(){ return *_value; };
        T * operator->(){ return _value; };

        p(dataContainerType, value){ return _value; }
    private:
        p(bool, IsValid){ return !!_value; }
    };
}

