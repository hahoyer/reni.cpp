#pragma once
#include "ValueCacheBase.h"

using std::function;

namespace HWLib
{
    template<typename T>
    class ValueCache final : public ValueCacheBase<T, std::unique_ptr<T>>{
        using baseType = ValueCacheBase<typename TData, typename TValue>;
        using thisType = ValueCache;

        virtual TValue GetValue() const override{ return TValue(new T(getValue())); };
        virtual TData GetResult(TValue & value) const override{ return *value; }
        virtual bool const IsValidValue(TValue & value) const override{ return !!value.get(); }
    public:
        function<T()> const getValue;
        ValueCache(function<T()> getValue) : getValue(getValue){}
    };


    template<typename T>
    class ValueCache<T&> final : public ValueCacheBase<CtrlRef<T>, CtrlPtr<T>>{
        using baseType = ValueCacheBase<typename TData, typename TValue>;
        using thisType = ValueCache;

        virtual TValue GetValue() const override{ return getValue(); };
        virtual TData GetResult(TValue & value) const override{ return value; }
        virtual bool const IsValidValue(TValue & value) const override{ return value.IsValid; }
    public:
        function<T&()> const getValue;
        ValueCache(function<T&()> getValue) : getValue(getValue){}
    };


    template<typename T>
    class ValueCache<CtrlRef<T>> final : public ValueCacheBase<CtrlRef<T>, CtrlPtr<T>>{
        using baseType = ValueCacheBase<typename TData, typename TValue>;
        using thisType = ValueCache;

        virtual TValue GetValue() const override{ return getValue(); };
        virtual TData GetResult(TValue & value) const override{ return value; }
        virtual bool const IsValidValue(TValue & value) const override{return !value.IsEmpty;}
    public:
        function<CtrlRef<T>()> const getValue;
        ValueCache(function<CtrlRef<T>()> getValue) : getValue(getValue){}
    };

}
