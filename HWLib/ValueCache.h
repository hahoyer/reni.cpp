#pragma once
#include "Properties.h"

namespace HWLib
{
    template<typename T>
    class ValueCache
    {
        mutable OptRef<T> _value;
        function<T()> const _getValue;
        mutable bool _isBusy;

    public:
        ValueCache(function<T()> getValue) 
            :_getValue(getValue)
            , _value()
            , _isBusy(false){}

        p_mutable(bool, IsValid){ return _value.IsValid; }
        p(bool, IsBusy){ return _isBusy; }

        T const& operator*()const { return *Value(); };
        T const* operator->()const { return Value(); };

    private:
        T const* Value()const 
        {
            Ensure();
            return _value.operator->();
        };

        void Ensure()const
        {
            assert(!_isBusy);
            if (_value.IsValid)
                return;
            _isBusy = true;
            _value = _getValue();
            _isBusy = false;
        }

        void Reset()const
        {
            assert(!_isBusy);
            _value = OptRef<T>();
        }

    };

    template<typename T>
    inline p_mutator_imlementation(ValueCache<T>, bool, IsValid)
    {
        if (value)
            Ensure();
        else
            Reset();
    }
}
