#pragma once

#include "DefaultAssignmentOperator.h"
#include "RefBase.h"

namespace HWLib{
    template<typename T>
    class RefCountContainer final{
        typedef RefCountContainer thisType;
        T * value;
    public:
        RefCountContainer(T *value);
        RefCountContainer(RefCountContainer<T> const&other);
        ~RefCountContainer();
        T const& operator*()const { return *value; };
        T const* operator->()const { return value; };
        T & operator*(){ return *value; };
        T * operator->(){ return value; };
        bool const operator==(thisType const&other)const{ return value == other.value; };
        DefaultAssignmentOperator;
    };

    template<typename T>
    class Ref final : public RefBase<T, RefCountContainer<T>>{
        typedef RefBase<T , RefCountContainer<T>> baseType;
        typedef Ref thisType;
        Ref() :baseType({}){}
    public:
        Ref(T &value) :baseType(&value){ }
        Ref(T *value) :baseType(value){ a_if_(value); }
        Ref(thisType const&other):baseType(other){};
        template<class TOther>
        Ref(Ref<TOther>&other) :baseType(*other){};

        DefaultAssignmentOperator;
    };
}