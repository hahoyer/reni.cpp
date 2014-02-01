#pragma once

#include "Properties.h"
#include <string>

namespace HWLib
{
    class String;

    class DumpableObject
    {
        mutable std::string _dumpString;
    public:
        DumpableObject(){ _dumpString = "<not set>"; }
        virtual_p(String, Dump) = 0;
    protected:
        void SetDumpString();
    };
}
