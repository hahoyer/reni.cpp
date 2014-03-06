#pragma once

#include "DefaultAssignmentOperator.h"
#include <boost/shared_ptr.hpp>
#include "RefBase.h"

namespace HWLib
{
    template<typename T>class OptRef;

    template<typename T>
    class Ref final : public RefBase<T, boost::shared_ptr<T>>{
        typedef RefBase<T, boost::shared_ptr<T>> baseType;
        typedef Ref thisType;
        friend class Ref<T>;
        friend class OptRef<T>;
        friend class Ref<T const>;
    public:
        Ref(T *value) :baseType(value){ }
        Ref(OptRef<T> const&other) : baseType(other){ a_if_(IsValid); };
        Ref(Ref<T> const&other) : baseType(other.value){ };
        Ref(Ref<T const> const&other) ;
        template<typename TOther>
        Ref(OptRef<TOther> const&other) : baseType(other){ a_if_(IsValid); };
        template<typename TOther>
        Ref(Ref<TOther> const&other) : baseType(other){ };

        DefaultAssignmentOperator;
    };

    template<typename T>
    class Ref<T const>final : public RefBase<T const, boost::shared_ptr<T const>>{
        typedef RefBase<T const, boost::shared_ptr<T const>> baseType;
        typedef Ref<T const> thisType;
    
    public:
        Ref(T const *value) :baseType(value){ a_if_(IsValid); }
        Ref(OptRef<T> const&other) : baseType(other){ a_if_(IsValid); };
        Ref(OptRef<T const> const&other) : baseType(other){ a_if_(IsValid); };
        Ref(Ref<T const> const&other) : baseType(other){ };

        DefaultAssignmentOperator;
        T const& operator*()const { return *value; };
        T const* operator->()const { return &*value; };

    private:
        T & operator*();
        T * operator->();
    };

    template<typename T>
    Ref<T>::Ref(Ref<T const> const&other) 
        : baseType(other.value){ };
}

