#pragma once
#include "Properties.h"
#include "CtrlRef.h"

namespace HWLib
{
    template<typename DataType, typename ValueType>
    class ValueCacheBase 
    {
    public:
        using TData = DataType;
        using TValue = ValueType;
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

        virtual TValue GetValue() const = 0;
        virtual TData GetResult(TValue & value) const = 0;
        virtual bool const IsValidValue(TValue & value) const = 0;

    private:
        void Ensure()const
        {
            a_if_(!isBusy);
            if (IsValidValue(value))
                return;
            isBusy = true;
            value = GetValue();
            isBusy = false;
        }

        void Reset()const
        {
            a_if_(!isBusy);
            value = nullptr;
        }

    };

    template<typename TData, typename TValue>
    inline void ValueCacheBase<TData, TValue>::p_mutator_name(IsValid)(bool const&value)
    {
        if (value)
            Ensure();
        else
            Reset();
    };
}
