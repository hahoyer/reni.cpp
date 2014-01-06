#pragma once

#include <functional>
#include "Properties.h"
using std::function;

namespace HWLib
{
    template<typename T> class IEnumerable
    {
    public: 
        int Count(function<bool(T)> selector)const;
        int Count()const{ return Count([](T){return true; }); }

        IEnumerable<T> const Concat(IEnumerable<T> const& other)const;

        p(int, Length){ return Count(); }
    };
}


