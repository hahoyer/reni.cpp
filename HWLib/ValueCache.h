#pragma once
#include "Properties.h"
#include "Ref.h"

namespace HWLib
{
    template<typename T>
    class ValueCache final
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

        p(Ref<T>, Value)
        {
            Ensure();
            return _value;
        };

    private:
        void Ensure()const
        {
            a_if_(!_isBusy);
            if (_value.IsValid)
                return;
            _isBusy = true;
            _value = new T(_getValue());
            _isBusy = false;
        }

        void Reset()const
        {
            a_if_(!_isBusy);
            _value = OptRef<T>();
        }

    };

    template<typename T>
    inline p_mutator_implementation(ValueCache<T>, bool, IsValid)
    {
        if (value)
            Ensure();
        else
            Reset();
    }
}
