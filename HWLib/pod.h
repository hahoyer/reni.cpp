#pragma once
#include "DefaultAssignmentOperator.h"

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

        DefaultAssignmentOperator;

        operator T const ()const{ return value; };
    };
}