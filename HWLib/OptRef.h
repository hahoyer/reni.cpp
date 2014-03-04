#pragma once

#include "DefaultAssignmentOperator.h"
#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace HWLib
{
    template<typename T>class Ref;

    template<typename T>
    class OptRef{
        using thisType = OptRef;
        friend class Ref<T>;
    public:
        using dataContainerType = boost::shared_ptr<T>;
    private:
        dataContainerType _value;
    public:
        OptRef() : _value() { }
        OptRef(T *value) :_value(value){ }
        OptRef(OptRef<T> const&other) : _value(other.value){ };
        OptRef(Ref<T> const&other) : _value(other.value){ };
        DefaultAssignmentOperator;
        template<typename TOther>
        OptRef(OptRef<TOther> const&other) : _value(other.value){ };

        T const& operator*()const { return *_value; };
        T const* operator->()const { return &*_value; };
        T & operator*(){ return *_value; };
        T * operator->(){ return &*_value; };
        p(bool, IsValid){ return !!_value.get(); }

        p(dataContainerType, value){ return _value; }
    };
}

