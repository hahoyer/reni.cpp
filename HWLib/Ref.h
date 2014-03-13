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
    bool IsEmptyValue(RefCountContainer<T> const&value){
        return !value.operator->();
    };


    template<typename T, bool isOptional = false>
    class Ref final : public RefBase<T, RefCountContainer<T>>{
        typedef RefBase<T , RefCountContainer<T>> baseType;
        typedef Ref thisType;
    public:
        Ref(T &value) :baseType(&value){ }
        Ref(T *value) :baseType(value){ a_if_(!IsEmpty); }
        Ref(Ref<T, true> const&other) :baseType(other){ a_if_(!IsEmpty); };
        Ref(Ref<T, false> const&other):baseType(other){};
        DefaultAssignmentOperator;
    };


    template<typename T>
    class Ref<T,true> final : public RefBase<T, RefCountContainer<T>>{
        typedef RefBase<T, RefCountContainer<T>> baseType;
        typedef Ref thisType;
    public:
        Ref() :baseType(null){ }
        Ref(T &value) :baseType(&value){ }
        Ref(T *value) :baseType(value){ }
        Ref(Ref<T, true> const&other) :baseType(other){ };
        Ref(Ref<T> const&other) :baseType(other){};
        DefaultAssignmentOperator;
        p(bool, IsEmpty){ return baseType::IsEmpty; }
    };

}