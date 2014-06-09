#pragma once

#include "DefaultAssignmentOperator.h"
#include <boost/shared_ptr.hpp>
#include "DumpableObject.h"
using boost::shared_ptr;

namespace HWLib
{
    template<typename T>class WeakRef;

    template<typename T>
    class WeakPtr
    {
        using thisType = WeakPtr;
    public:
        using dataContainerType = T*;
        friend class WeakRef<T>;
        friend class WeakRef<T const>;
        friend class WeakPtr<T const>;
    private:
        dataContainerType value;
    public:
        WeakPtr() : value() { }
        WeakPtr(T &value) :value(&value){ }
        WeakPtr(T *value) :value(value){ }
        WeakPtr(WeakPtr<T const> const&value) : value(value.value){ };
        WeakPtr(WeakPtr<T     > const&value) : value(value.value){ };
        WeakPtr(WeakRef  <T const> const&value) : value(value.value){ };
        WeakPtr(WeakRef <T      > const&value) : value(value.value){ };
        DefaultAssignmentOperator;

        T const& operator*()const { return *value; };
        T const* operator->()const { return value; };
        T     & operator*()       { return *value; };
        T    * operator->()      { return  value; };
        p(bool, IsEmpty){ return !value; }

        bool const operator==(thisType const&other)const
        {
            if(IsEmpty)
                return other.IsEmpty;
            return *value == *other.value;
        };

        template <class TOther>
        friend TOther operator||(thisType left, TOther right) {
            if(left.IsEmpty)
                return right;
            return left;
        }
        template <class TOther>
        friend TOther operator||(thisType left, std::function<TOther()> right) {
            if(left.IsEmpty)
                return right();
            return left;
        }

    };

    template<typename T>
    class WeakPtr<T const>
    {
        using thisType = WeakPtr;
    public:
        using dataContainerType = T const*;
        friend class WeakRef<T const>;
        friend class WeakRef<T>;
        friend class WeakPtr<T>;
    private:
        dataContainerType value;
    public:
        WeakPtr() : value() { }
        WeakPtr(T const*value) :value(value){ }
        WeakPtr(WeakPtr<T const> const&value) : value(value.value){ };
        WeakPtr(WeakPtr<T     > const&value) : value(value.value){ };
        WeakPtr(WeakRef  <T const> const&value) : value(value.value){ };
        WeakPtr(WeakRef <T      > const&value) : value(value.value){ };
        DefaultAssignmentOperator;

        T const& operator*()const { return *value; };
        T const* operator->()const { return value; };
        p(bool, IsEmpty){ return !value; }
    };
}

