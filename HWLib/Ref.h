#pragma once

#include <memory>
using std::shared_ptr;

namespace HWLib
{
    template<typename T>
    class Ref : public shared_ptr<T const>
    {
        typedef shared_ptr<T const> baseType;
    public:
        Ref(T const&data) :baseType(&data){}
        Ref(Ref<T const> const&data) :baseType(data.get()){}
    };

    template<typename T>
    class OptRef : public shared_ptr<T const>
    {
        typedef shared_ptr<T const> baseType;
    public:
        OptRef() :baseType(){}
        OptRef(T const&data) :baseType(&data){}
        OptRef(Ref<T const> const&data) :baseType(data.get()){}
        OptRef(OptRef<T const> const&data) :baseType(data.get()){}
        p(bool, IsValid){ return !get(); }
    };

    template<typename T>
    class Var : public shared_ptr<T >
    {
        typedef shared_ptr<T > baseType;
    public:
        Var(T &data) :baseType(&data){}
        Var(T*data) :baseType(data){}
    };

}

