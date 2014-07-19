#pragma once

#include "DefaultAssignmentOperator.h"
#include "Union.h"
using namespace std;

namespace HWLib
{
    class Nothing{};

    template<class T>
    class Optional final 
    {
        using thisType = Optional;
        Union<T, Nothing> data;
    public:
        Optional() : data(Nothing()){};
        Optional(T const&data) : data(data){};
        Optional(thisType const&other) : data(other.data){};
        DefaultAssignmentOperator;
        p(bool, IsValid){ return data.is<T>(); };
        p(bool, IsEmpty){ return data.is<Nothing>(); };

        p(T, Value){
            a_if_(IsValid);
            return data.get<T>();
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

    };


    class String;

    template <typename T>
    inline String const Dump(Optional<T> const& target)
    {
        if(target.IsValid)
            return "?{ " + HWLib::Dump(target.Value) + " }";
        return "?{}";
    }
}

