#pragma once

#include "BreakHandling.h"
#include "String.h"
#include "Common.h"
#include "DumpableObject.h"
using boost::shared_ptr;

namespace HWLib
{

    template<typename T>
    class Ref : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = Ref<T>;
    protected:
        shared_ptr<T> value;
        Ref() : value() { SetDumpString(); }
        Ref(shared_ptr<T> value) :value(value){ SetDumpString(); }
    public:
        Ref(T *value) :value(value){ SetDumpString(); }
        Ref(T const& value) :value(new T(value)){ SetDumpString(); }
        Ref(Ref<T> const&value) :value(value.value){ }
        Ref(OptRef<T> const&value) :value(value.value){ assert(!!this->value.get()); SetDumpString(); }
        virtual ~Ref(){};
        DefaultAssignmentOperator;

        T const& operator*()const { return value.operator*(); };
        T const* operator->()const { return value.operator->(); };
        T & operator*(){ return value.operator*(); };
        T * operator->(){ return value.operator->(); };

        virtual p_function(Array<String>, DumpData)override;
        virtual p_function(String, DumpHeader)override;
    protected:
        void SetDumpString(){
            if (Features<T>::EnableDumpFromRef())
                baseType::SetDumpString();
        };
    };

    template<typename T>
    class OptRef final : public Ref<T>
    {
        using thisType = OptRef<T>;
        using baseType = Ref<T>;
    public:
        OptRef(){};
        OptRef(decltype(null)) : thisType(){ }
        OptRef(T *value) :baseType(value){ }
        OptRef(T const&value) :baseType(value){ }
        OptRef(Ref<T> const&other) :baseType(other){ }
        OptRef(OptRef<T> const&other) :baseType(other.value){ }
        virtual ~OptRef(){};

        p(bool, IsValid){ return !!value.get(); }
        DefaultAssignmentOperator;

        friend OptRef<T> operator||(OptRef<T> left, function<T*()> right)
        {
            return left.IsValid ? left : right();
        }
    };

    template<typename T>
    class Constants final{};

    template<>
    class Constants <int>
    {
    public:
        static int const NotValid = 0x80000000;
    };

    template<typename T>
    class Optional final
    {
        using thisType = Optional;

        T const value;
    public:
        Optional() : value(Constants<T>::NotValid){}
        Optional(decltype(null)) : value(Constants<T>::NotValid){}
        Optional(T const value) :value(value){}

        p(bool, IsValid){ return value != Constants<T>::NotValid; }
        
        DefaultAssignmentOperator;

        p(T, Value)
        {
            assert(IsValid);
            return value;
        };

        operator T const ()const{ return Value; };

        friend Optional<T> operator||(Optional<T> left, function<Optional<T>()> right)
        {
            if (left.IsValid)
                return left;
            return right();
        }
    };

}

