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
        dataContainerType value;
    public:
        WeakRef(T &value) :value(&value){ }
        WeakRef(T *value) :value(value){ a_if_(value); }
        WeakRef(WeakRef<T> const&other) : value(other.value){ };
        
        friend class WeakRef;
        template<typename TOther>
        WeakRef(WeakRef<TOther> const&other) : value(other.value){ };

        DefaultAssignmentOperator;

        bool const operator==(thisType const&other)const{ return value == other.value; }
        T const& operator*()const { return *value; };
        T const* operator->()const { return value; };
        T & operator*(){ return *value; };
        T * operator->(){ return value; };
    };
}

