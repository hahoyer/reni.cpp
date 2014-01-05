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
        p(int, Length){ return Count(); }
    };
}


