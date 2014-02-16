#pragma once

#include "Properties.h"
#include <string>

namespace HWLib
{
    class String;

    class DumpableObjectBase
    {
        mutable std::string dumpString;
        mutable std::string dumpShortString;
    public:
        virtual_p(String, DumpLong) = 0;
        virtual_p(String, DumpShort);
        p(String, Dump);
    protected:
        DumpableObjectBase();
        virtual ~DumpableObjectBase(){};
        void SetDumpString();
    };

    template<typename T> class Array;

    class DumpableObject: public DumpableObjectBase
    {
        using baseType = DumpableObjectBase;
        using thisType = DumpableObject;
    public:
        int const ObjectId;
    protected:
        DumpableObject();
        DumpableObject(int objectId);
        virtual_p(Array<String>, DumpData) = 0;
        virtual_p(String, DumpHeader);
    public:
        override_p_function(String, DumpShort);
        p(bool, IsInDump){ return isInDump; }
        override_p_function(String, DumpLong)final;
    private:
        mutable bool isInDump;
    };

};

