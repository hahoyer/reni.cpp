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
        DefaultAssignmentOperator;
    };

    template<class T>
    bool IsValidValue(RefCountContainer<T> const&value){
        return !!value.operator->();
    }

    template<typename T>
    class Ref;

    
    template<typename T>
    class Ptr final : public RefBase<T, RefCountContainer<T>>{
        typedef RefBase<T, RefCountContainer<T>> baseType;
        typedef Ptr thisType;
    public:
        Ptr() :baseType(null){};
        Ptr(T *value) :baseType(value){};
        Ptr(Ptr<T> const&other) : baseType(other){};
        Ptr(Ref<T > const&other) : baseType(other){};
        DefaultAssignmentOperator;
        p(bool, IsValid){ return baseType::IsValid; }
    };

    
    template<typename T>
    class Ref final : public RefBase<T, RefCountContainer<T>>{
        typedef RefBase<T , RefCountContainer<T>> baseType;
        typedef Ref thisType;
    public:
        Ref(T *value) :baseType(value){a_if_(IsValid);}
        Ref(Ptr<T> const&other):baseType(other){a_if_(IsValid);};
        Ref(Ref<T> const&other):baseType(other){};
        DefaultAssignmentOperator;
    };
}