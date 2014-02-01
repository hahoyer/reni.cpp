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
        virtual_p(String, Dump) = 0;
    protected:
        DumpableObject(){ _dumpString = "<not set>"; }
        void SetDumpString();
    };

}
