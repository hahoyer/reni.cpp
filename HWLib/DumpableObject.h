#pragma once

#include "Properties.h"
#include <string>

namespace HWLib{
    class String;
    template<typename T>
    class Array;


    template<typename TBase, typename TRealm = TBase>
    class WithId: public TBase{
        using baseType = TBase;
        using thisType = WithId;
    public:
        static int NextObjectId;
        int const ObjectId;
        WithId() : ObjectId(NextObjectId++){};
        p_function(String, DumpHeader) override;
    };


    class DumpableObject{
        using thisType = DumpableObject;
        mutable std::string dumpString;
        mutable std::string dumpShortString;
    protected:
        DumpableObject();                     
        virtual ~DumpableObject(){};
        virtual_p(Array<String>, DumpData) = 0;
        virtual_p(String, DumpHeader);
        void SetDumpString();
    public:
        p(String, Dump);
        virtual_p(String, DumpShort);
        p(bool, IsInDump){return isInDump;}
        p(String, DumpLong);
    private:
        mutable bool isInDump;
    };
};


template<typename TBase, typename TRealm>
int HWLib::WithId<TBase, TRealm>::NextObjectId = 0;