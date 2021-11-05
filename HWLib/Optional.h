#pragma once

#include "DefaultOperators.h"
#include "Properties.h"
#include "Union.h"
#include <functional>
using namespace std;

namespace HWLib
{
    template<class T>
    class Optional
    {
        using thisType = Optional;
        using targetType = typename remove_const<T>::type;
        Union<targetType, Union<>> data;
    public:
        Optional() : data(Union<>()){ Initialize(); };
        Optional(T const&data) : data(data){ Initialize(); };
        Optional(thisType const&other) : data(other.data){ Initialize(); };
        HW_DO_PLACEMENT_ASSIGN;
        p(bool, IsValid){ return data.is<targetType>(); };
        p(bool, IsEmpty){ return data.is<Union<>>(); };

        p_definition(T,Value); 
        T const p_name(Value)()const
        {
            a_if_(IsValid);
            return data.get<targetType>();
        };

        T p_name(Value)()
        {
            a_if_(IsValid);
            return data.get<targetType>();
        };

        operator T const ()const{ return Value; };

        thisType const operator||(Optional<T> right)const
        {
            if (IsValid)
                return *this;
            return right;
        }

        T const operator||(T right)const
        {
            if (IsValid)
                return Value;
            return right;
        }

        thisType const operator||(function<Optional<T>()> right)const
        {
            if (IsValid)
                return *this;
            return right();
        }

        bool const operator==(Optional<T> const& other)const
        {
            if (IsEmpty)
                return other.IsEmpty;
            return Value == other.Value;
        };
    private:
        T const* _data;

        void Initialize()
        {
            _data = nullptr;
            if(IsEmpty)
                return;
            _data = &data.get_unchecked<T>();
        };
    };

    template <typename T>
    inline string Dump(Optional<T> const& target)
    {
        if(target.IsValid)
            return "?{ " + HWLib::Dump(target.Value) + " }";
        return "?{}";
    }
}

