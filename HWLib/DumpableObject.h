#pragma once

#include "Properties.h"
#include <string>

namespace HWLib
{
    class String;

    template <typename T>
    struct Features{
        static inline bool EnableDumpFromRef(){ return false; };
        static String const DumpToStringShort(T const&value);
    };

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
        p_function(String, DumpShort)override;
        p(bool, IsInDump){ return isInDump; }
    private:
        p_function(String, Dump)override final;
        mutable bool isInDump;
    };

};
