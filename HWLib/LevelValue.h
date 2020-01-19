#pragma once

#include "Properties.h"

namespace HWLib
{
    template<typename T>
    class LevelValue
    {
        T & value;
        T const oldValue;
    public:
        LevelValue(T & targetValue, T newValue)
            : value(targetValue)
            , oldValue(targetValue)
        {
            value = newValue;
        }
        LevelValue(T & targetValue)
            : value(targetValue)
            , oldValue(targetValue)
        {
        }

        ~LevelValue(){ value = oldValue; }
    };
};


