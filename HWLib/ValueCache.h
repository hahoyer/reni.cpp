#pragma once
#include "Properties.h"
#include "Ref.h"
//#include <memory>

namespace HWLib
{
    template<typename DataType, typename ValueType, typename GetValueType>
    class ValueCacheBase 
    {
    public:
        using TData = DataType;
        using TValue = ValueType;
        using TGetValue = GetValueType;
    private:
        using thisType = ValueCacheBase;

        mutable TValue value;
        mutable bool isBusy;
    public:
        ValueCacheBase() 
            : value(null)
            , isBusy(false){}

        p_mutable(bool, IsValid){ return IsValidValue(value); }
        p(bool, IsBusy){ return isBusy; }

        mutable_p(TData, Value)const
        {
            Ensure();
            return GetResult(value);
        };

        virtual TGetValue GetValue() const = 0;
        virtual TData GetResult(TValue & value) const = 0;
        virtual bool const IsValidValue(TValue & value) const = 0;

    private:
        void Ensure()const
        {
            a_if_(!isBusy);
            if (IsValidValue(value))
                return;
            isBusy = true;
            value = TValue(&GetValue());
            isBusy = false;
        }

        void Reset()const
        {
            a_if_(!isBusy);
            value = nullptr;
        }

    };


    template<typename TData, typename TValue, typename TGetValue>
    inline void ValueCacheBase<TData, TValue, TGetValue>::p_mutator_name(IsValid)(bool const&value)
    {
        if (value)
            Ensure();
        else
            Reset();
    };


    template<typename T>
    class ValueCache final : public ValueCacheBase<T, std::unique_ptr<T>, T&>{
        using baseType = ValueCacheBase<typename TData, typename TValue, typename TGetValue>;
        using thisType = ValueCache;

        virtual TGetValue GetValue() const override{ return *new T(getValue()); };
        virtual TData GetResult(TValue & value) const override{ return *value; }
        virtual bool const IsValidValue(TValue & value) const override{ return !!value.get(); }
    public:
        function<T()> const getValue;
        ValueCache(function<T()> getValue) : getValue(getValue){}
    };


    template<typename T>
    class ValueCache<T&> final : public ValueCacheBase<Ref<T>, OptRef<T>, T&>{
        using baseType = ValueCacheBase<typename TData, typename TValue, typename TGetValue>;
        using thisType = ValueCache;

        virtual TGetValue GetValue() const override{return getValue();};
        virtual TData GetResult(TValue & value) const override{ return value; }
        virtual bool const IsValidValue(TValue & value) const override{ return value.IsValid; }
    public:
        function<TGetValue()> const getValue;
        ValueCache(function<TGetValue()> getValue) : getValue(getValue){}
    };


    template<typename T>
    class ValueCache<Ref<T>> final : public ValueCacheBase<Ref<T>, OptRef<T>, Ref<T>>{
        using baseType = ValueCacheBase<typename TData, typename TValue, typename TGetValue>;
        using thisType = ValueCache;

        virtual TGetValue GetValue() const override{return getValue();};
        virtual TData GetResult(TValue & value) const override{ return value; }
        virtual bool const IsValidValue(TValue & value) const override{return value.IsValid;}
    public:
        function<TGetValue()> const getValue;
        ValueCache(function<TGetValue()> getValue) : getValue(getValue){}
    };

}
