#pragma once

#include "DefaultAssignmentOperator.h"
#include "Common.h"
#include <memory>
using std::shared_ptr;

namespace HWLib
{
    template<typename T>
    class Ref 
    {
        using thisType = Ref<T>;
    protected:
        shared_ptr<T const> const _data;
        Ref(): _data(nullptr) {}
    public:
        explicit Ref(T const*data) :_data(data){}
        Ref(T const&data) :_data(new T(data)){}
        Ref(Ref<T const> const&data) :_data(data._data){}
        DefaultAssignmentOperator;

        T const& operator*()const { return _data.operator*(); };
        T const* operator->()const { return _data.operator->(); };
    };

    template<typename T>
    class OptRef : public Ref<T>
    {
        using thisType = OptRef<T>;
        using baseType = Ref<T>;
    public:
        OptRef() = default;
        explicit OptRef(T const*data) :baseType(data){}
        OptRef(T const&data) :baseType(data){}
        OptRef(Ref<T const> const&data) :baseType(data){}
        OptRef(OptRef<T const> const&data) :baseType(data){}
        p(bool, IsValid){ return !!_data.get(); }
        DefaultAssignmentOperator;
    };

    template<typename T>
    class Var : public shared_ptr<T>
    {
        typedef shared_ptr<T> baseType;
    public:
        Var(T&data) : baseType(&data){}
        Var(T*data) : baseType(data){}
        Var(Var<T> const&data) : baseType(data){}
    };

}

