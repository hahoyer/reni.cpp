#pragma once

#include "DefaultAssignmentOperator.h"
#include <boost/shared_ptr.hpp>
#include "RefBase.h"

namespace HWLib
{
    template<typename T>class CtrlPtr;

    template<typename T>
    class CtrlRef final : public RefBase<T, boost::shared_ptr<T>>{
        typedef RefBase<T, boost::shared_ptr<T>> baseType;
        typedef CtrlRef thisType;
        friend class CtrlRef<T>;
        friend class CtrlPtr<T>;
        friend class CtrlRef<T const>;
    public:
        CtrlRef(T *value) :baseType(value){ a_if_(IsValid); }
        CtrlRef(CtrlPtr<T> const&other) : baseType(other){ a_if_(IsValid); };
        CtrlRef(CtrlRef<T> const&other) : baseType(other.value){ };
        CtrlRef(CtrlRef<T const> const&other) ;
        template<typename TOther>
        CtrlRef(CtrlPtr<TOther> const&other) : baseType(other){ a_if_(IsValid); };
        template<typename TOther>
        CtrlRef(CtrlRef<TOther> const&other) : baseType(other){ };

        DefaultAssignmentOperator;
    };

    template<typename T>
    class CtrlRef<T const>final : public RefBase<T const, boost::shared_ptr<T const>>{
        typedef RefBase<T const, boost::shared_ptr<T const>> baseType;
        typedef CtrlRef<T const> thisType;
    
    public:
        CtrlRef(T const *value) :baseType(value){ a_if_(IsValid); }
        CtrlRef(CtrlPtr<T> const&other) : baseType(other){ a_if_(IsValid); };
        CtrlRef(CtrlPtr<T const> const&other) : baseType(other){ a_if_(IsValid); };
        CtrlRef(CtrlRef<T const> const&other) : baseType(other){ };

        DefaultAssignmentOperator;
        T const& operator*()const { return *value; };
        T const* operator->()const { return &*value; };

    private:
        T & operator*();
        T * operator->();
    };

    template<typename T>
    CtrlRef<T>::CtrlRef(CtrlRef<T const> const&other) 
        : baseType(other.value){ };
}

