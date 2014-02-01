#pragma once

#include "Properties.h"
#include <string>

namespace HWLib
{
    class String;

    class Object
    {
        mutable std::string _dumpString;
    public:
        Object(){ _dumpString = ""; }
        virtual_p(String, Dump) = 0;
    protected:
        void SetDumpString();
    };
}
