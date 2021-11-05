#pragma once
#include "DefaultOperators.h"

namespace HWLib
{
    template <class T>
    class pod
    {
        using thisType = pod;
        T const value;
    public:

        pod(T const value) 
            : value(value)
        {
        }

        pod(pod<T> const& other) 
            : value(other.value)
        {
        }

        HW_DO_PLACEMENT_ASSIGN;

        operator T const ()const{ return value; };
    };
}