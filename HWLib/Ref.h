#pragma once

#include "BreakHandling.h"
#include "String.h"
#include "Common.h"
using boost::shared_ptr;

namespace HWLib
{
    template<typename T>

    class Ref
    {
        using thisType = Ref<T>;
    protected:
        shared_ptr<T> value;
        Ref() : value(nullptr) {}
        Ref(shared_ptr<T> value) :value(value){}
    public:
        Ref(T *value) :value(value){}
        Ref(T & value) :value(new T(value)){}
        Ref(Ref<T> const&value) :value(value.value){}
        Ref(OptRef<T> const&value) :value(value.value){ assert(!!this->value.get()); }
        virtual ~Ref(){};
        DefaultAssignmentOperator;

        T const& operator*()const { return value.operator*(); };
        T const* operator->()const { return value.operator->(); };
        T & operator*(){ return value.operator*(); };
        T * operator->(){ return value.operator->(); };
    };

    template<typename T>
    String const DumpToString(Ref<T> const&target);

    template<typename T>
    class OptRef final : public Ref<T>
    {
        using thisType = OptRef<T>;
        using baseType = Ref<T>;
    public:
        OptRef() = default;
        OptRef(decltype(null)) : thisType(){}
        OptRef(T *value) :baseType(value){}
        OptRef(T &value) :baseType(value){}
        OptRef(Ref<T> const&other) :baseType(other){}
        OptRef(OptRef<T> const&other) :baseType(other.value){}
        virtual ~OptRef(){};

        p(bool, IsValid){ return !!value.get(); }
        DefaultAssignmentOperator;

        friend OptRef<T> operator||(OptRef<T> left, function<T*()> right)
        {
            return left.IsValid ? left : right();
        }
    };

    template<typename T>
    String const DumpToString(OptRef<T> const&target);

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

    template<typename T>
    String const DumpToString(Optional<T> const&target);


}

