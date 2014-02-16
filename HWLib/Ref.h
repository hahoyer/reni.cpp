#pragma once

#include "String.h"
#include "Common.h"
#include "DumpableObject.h"
#include "DumpToString.h"
#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace HWLib
{
    template<typename T>
    class Pointer 
    {
        using thisType = Pointer<T>;
    protected:
        shared_ptr<T> value;
        Pointer() : value() { }
        Pointer(shared_ptr<T> value) :value(value){ }
    public:
        Pointer(T *value) :value(value){ }
        Pointer(Pointer <T> const&value) : value(value.value){ };

        DefaultAssignmentOperator;

        T const& operator*()const { return value.operator*(); };
        T const* operator->()const { return value.operator->(); };
        T & operator*(){ return value.operator*(); };
        T * operator->(){ return value.operator->(); };
    };


    template<typename T>
    struct default_ref_traits{
        static bool const EnableSetDumpString = false;
        static String const DumpValue(T const&value);
        static String const DumpValueHeader(T const&value);
        static String const DumpValueShort(T const&value);
    };


    template<typename T>
    class Ref : public DumpableObject, public Pointer<T>
    {
        using baseType = Pointer<T>;
        using thisType = Ref<T>;
    public:
        struct traits : public default_ref_traits<T>{};
    protected:
        Ref() : baseType() { SetDumpString(); }
        Ref(shared_ptr<T> value) :baseType(value){ SetDumpString(); }
    public:
        Ref(T *value) :baseType(value){ SetDumpString(); }
        Ref(T const& value) :baseType(new T(value)){ SetDumpString(); };
        Ref(Ref<T> const&value) : baseType(value){ };
        Ref(OptRef<T> const&value) : baseType(value){ a_if_(!!this->value.get()); SetDumpString(); }

        virtual ~Ref(){};
        DefaultAssignmentOperator;

        override_p_function(Array<String>, DumpData);
        override_p_function(String, DumpHeader);
        override_p_function(String, DumpShort);
    protected:
        void SetDumpString(){
            if (traits::EnableSetDumpString || !value.get())
                DumpableObject::SetDumpString();
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

        override_p_function(String, DumpHeader);
        override_p_function(String, DumpShort);

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
            a_if_(IsValid);
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

