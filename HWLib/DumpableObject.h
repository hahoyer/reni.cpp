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
        virtual_p(String, Dump) = 0;
        virtual_p(String, DumpShort);
    protected:
        DumpableObjectBase();
        virtual ~DumpableObjectBase(){};
        void SetDumpString();
    };

    template<typename T> class Array;

    struct DumpableObject: public DumpableObjectBase
    {
        using baseType = DumpableObjectBase;
        using thisType = DumpableObject;
        int const ObjectId;
    protected:
        DumpableObject();
        DumpableObject(int objectId);
        virtual_p(Array<String>, DumpData) = 0;
        virtual_p(String, DumpHeader);
    public:
        p(bool, IsInDump){ return isInDump; }
    private:
        p_function(String, Dump)override final;
        p_function(String, DumpShort)override final;
        mutable bool isInDump;
    };
};
