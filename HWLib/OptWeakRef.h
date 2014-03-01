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
    private:
        dataContainerType _value;
    public:
        OptWeakRef() : _value() { }
        OptWeakRef(T *value) :_value(value){ }
        OptWeakRef(OptWeakRef<T> const&value) : _value(value.value){ };
        OptWeakRef(WeakRef<T> const&value) : _value(value.value){ };
        template<typename TOther>
        OptWeakRef(OptWeakRef<TOther> const&other) : _value(other.value){ };
        DefaultAssignmentOperator;

        T const& operator*()const { return *_value; };
        T const* operator->()const { return _value; };
        T & operator*(){ return *_value; };
        T * operator->(){ return _value; };
        p(bool, IsValid){ return !!_value; }

        p(dataContainerType, value){ return _value; }
    };
}

