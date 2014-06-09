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

    template<class T>
    bool IsEmptyValue(RefCountContainer<T> const&value){
        return !value.operator->();
    };

    template<class T>  class Constants;
    
    template<typename T>
    class Ref final : public RefBase<T, RefCountContainer<T>>{
        typedef RefBase<T , RefCountContainer<T>> baseType;
        typedef Ref thisType;
        Ref() :baseType({}){}
    public:
        Ref(T &value) :baseType(&value){ }
        Ref(T *value) :baseType(value){ a_if_(!IsEmpty); }
        Ref(thisType const&other):baseType(other){};
        template<class TOther>
        Ref(Ref<TOther>&other) :baseType(*other){};

        DefaultAssignmentOperator;
        friend class Constants<Ref<T>>;
    };


    template<class T>
    class Constants <Ref<T>>{
    public:
        static Ref<T> const NotValid;
    };

    template<class T>
    Ref<T> const Constants<Ref<T>>::NotValid = Ref<T>();
}