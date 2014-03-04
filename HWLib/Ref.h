#pragma once

#include "DefaultAssignmentOperator.h"
#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace HWLib
{
    template<typename T>class OptRef;

    template<typename T>
    class Ref{
        using thisType = Ref;
    public:
        using dataContainerType = boost::shared_ptr<T>;
    private:
        dataContainerType _value;
    public:
        Ref(T *value) :_value(value){ a_if_(IsValid); }
        Ref(OptRef<T> const&other) : _value(other.value){ a_if_(IsValid); };
        Ref(Ref<T> const&other) : _value(other.value){ };
        Ref(Ref<T const> const&other);

        DefaultAssignmentOperator;

        T const& operator*()const { return *_value; };
        T const* operator->()const { return &*_value; };
        T & operator*(){ return *_value; };
        T * operator->(){ return &*_value; };

        mutable_p(dataContainerType, value)const{ return _value; }
    private:
        p(bool, IsValid){ return !!_value.get(); }
    };

    template<typename T>
    class Ref<T const>{
        using thisType = Ref;
    public:
        using dataContainerType = boost::shared_ptr<T const>;
    private:
        dataContainerType _value;
    public:
        Ref(T const *value) :_value(value){ a_if_(IsValid); }
        Ref(OptRef<T> const&other) : _value(other.value){ a_if_(IsValid); };
        Ref(OptRef<T const> const&other) : _value(other.value){ a_if_(IsValid); };
        Ref(Ref<T> const&other) : _value(other.value){ };
        Ref(Ref<T const> const&other) : _value(other.value){ };

        DefaultAssignmentOperator;

        T const& operator*()const { return *_value; };
        T const* operator->()const { return &*_value; };

        mutable_p(dataContainerType, value)const{ return _value; }
    private:
        p(bool, IsValid){ return !!_value.get(); }
    };

    template<typename T>
    Ref<T>::Ref(Ref<T const> const&other) 
        : _value(other.value){ };
}

