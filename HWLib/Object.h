#pragma once

#include "Properties.h"
#include <string>

namespace HWLib
{
    class Object
    {
        mutable std::string _dumpString;
    public:
        Object(){ _dumpString = "Hallo"; }
        virtual_p(std::string, Dump) = 0;
    protected:
        void SetDumpString(){ _dumpString = Dump; }
    };
}
