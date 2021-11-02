#pragma once
#include "Optional.h"

using std::function;

namespace HWLib
{
    template<typename T>
    class ValueCache final 
    {
        using thisType = ValueCache;

        mutable Optional<T> value;
        mutable bool isBusy;
    public:
        function<T()> const getValue;

        ValueCache(function<T()> getValue) 
            : getValue(getValue)
            , isBusy(false)
        {
        }

        p_mutable(bool, IsValid){ return value.IsValid; }
        p(bool, IsBusy){ return isBusy; }

        p_nonconst(T, Value)const
        {
            Ensure();
            return value.Value;
        };
    private:
        void Ensure()const
        {
            a_if_(!isBusy);
            if(value.IsValid)
                return;
            isBusy = true;
            value = getValue();
            isBusy = false;
        }

        void Reset()const
        {
            a_if_(!isBusy);
            value = {};
        }

    };

    template<typename T>
    inline void ValueCache<T>::p_mutator_name(IsValid)(bool const&value)
    {
        if(value)
            Ensure();
        else
            Reset();
    };
}
