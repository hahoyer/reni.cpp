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
        shared_ptr<T> _data;
        Ref(): _data(nullptr) {}
    public:
        Ref(T *data) :_data(data){}
        explicit Ref(T const& data) :_data(new T(data)){}
        Ref(Ref<T> const&data) :_data(data._data){}
        DefaultAssignmentOperator;

        T const& operator*()const { return _data.operator*(); };
        T const* operator->()const { return _data.operator->(); };
        T & operator*(){ return _data.operator*(); };
        T * operator->(){ return _data.operator->(); };
    };

    template<typename T>
    class OptRef : public Ref<T>
    {
        using thisType = OptRef<T>;
        using baseType = Ref<T>;
    public:
        OptRef() = default;
        OptRef(T *data) :baseType(data){}
        explicit OptRef(T const&data) :baseType(new T(data)){}
        OptRef(Ref<T> &data) :baseType(data){}
        OptRef(OptRef<T> const&data) :baseType(data){}
        p(bool, IsValid){ return !!_data.get(); }
        DefaultAssignmentOperator;
    };

    template<typename T>
    class VarY : public shared_ptr<T>
    {
        typedef shared_ptr<T> baseType;
    public:
        VarY(T&data) : baseType(&data){}
        VarY(T*data) : baseType(data){}
        VarY(VarY<T> const&data) : baseType(data){}
    };

}

