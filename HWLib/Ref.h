#pragma once

#include "DefaultOperators.h"
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
        HW_DO_PLACEMENT_ASSIGN;
    };

    template<typename T>
    class Ref final : public RefBase<T, RefCountContainer<T>>{
        typedef RefBase<T , RefCountContainer<T>> baseType;
        typedef Ref thisType;
        Ref() :baseType({}){}
    public:
        Ref(T &value) :baseType(&value){ }
        Ref(T *value) :baseType(value){ HW_ASSERT_(value); }
        Ref(thisType const&other):baseType(other){};
        template<class TOther>
        Ref(Ref<TOther>&other) :baseType(*other){};

        HW_DO_PLACEMENT_ASSIGN;
    };
}