#pragma once

#include "Properties.h"
#include <string>

namespace HWLib
{
    class String;

    class DumpableObject
    {
        mutable std::string _dumpString;
        mutable std::string _dumpShortString;
    public:
        virtual_p(String, Dump) = 0;
        virtual_p(String, DumpShort);

    protected:
        DumpableObject();
        virtual ~DumpableObject(){};
        void SetDumpString();
    };

}
