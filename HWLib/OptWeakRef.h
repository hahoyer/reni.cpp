#pragma once

#include "DefaultAssignmentOperator.h"
#include <boost/shared_ptr.hpp>
#include "DumpableObject.h"
using boost::shared_ptr;

namespace HWLib
{
    template<typename T>class WeakRef;

    template<typename T>
    class OptWeakRef
    {
        using thisType = OptWeakRef;
    public:
        using dataContainerType = T*;
        friend class WeakRef<T>;
        friend class WeakRef<T const>;
        friend class OptWeakRef<T const>;
    private:
        dataContainerType value;
    public:
        OptWeakRef() : value() { }
        OptWeakRef(T *value) :value(value){ }
        OptWeakRef(OptWeakRef<T const> const&value) : value(value.value){ };
        OptWeakRef(OptWeakRef<T     > const&value) : value(value.value){ };
        OptWeakRef(WeakRef  <T const> const&value) : value(value.value){ };
        OptWeakRef(WeakRef <T      > const&value) : value(value.value){ };
        DefaultAssignmentOperator;

        T const& operator*()const { return *value; };
        T const* operator->()const { return value; };
        T     & operator*()       { return *value; };
        T    * operator->()      { return  value; };
        p(bool, IsValid){ return !!value; }
    };

    template<typename T>
    class OptWeakRef<T const>
    {
        using thisType = OptWeakRef;
    public:
        using dataContainerType = T const*;
        friend class WeakRef<T const>;
        friend class WeakRef<T>;
        friend class OptWeakRef<T>;
    private:
        dataContainerType value;
    public:
        OptWeakRef() : value() { }
        OptWeakRef(T const*value) :value(value){ }
        OptWeakRef(OptWeakRef<T const> const&value) : value(value.value){ };
        OptWeakRef(OptWeakRef<T     > const&value) : value(value.value){ };
        OptWeakRef(WeakRef  <T const> const&value) : value(value.value){ };
        OptWeakRef(WeakRef <T      > const&value) : value(value.value){ };
        DefaultAssignmentOperator;

        T const& operator*()const { return *value; };
        T const* operator->()const { return value; };
        p(bool, IsValid){ return !!value; }
    };
}

