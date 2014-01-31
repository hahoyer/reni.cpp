#pragma once

#include "Properties.h"
#include <string>

namespace HWLib
{
    class Object
    {
        mutable std::string _dumpString;
    public:
        Object(){ _dumpString = ""; }
        virtual_p(std::string, Dump) = 0;
    protected:
        void SetDumpString();
    };
}
