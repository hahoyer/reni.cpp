#pragma once

#include "BreakHandling.h"
#include "Properties.h"
#include <string>
#include <thread>

namespace HWLib{
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
        p_function(std::string, DumpHeader) override;
    };


    class DumpableObject{
        using thisType = DumpableObject;
        mutable bool isDumpStringValid;
        mutable std::string dumpString;
        mutable std::string dumpShortString;
        mutable bool isInDump;

        friend class SetDumpStringQueueEntry;
        SetDumpStringQueueEntry* SetDumpStringToDo;
    protected:
        DumpableObject();                     
        DumpableObject(DumpableObject const&);
        virtual ~DumpableObject();
        p_virtual(Array<std::string>, DumpData) = 0;
        p_virtual(std::string, DumpHeader);
    public:
        void SetDumpString();
        p(std::string, Dump);
        p_virtual(std::string, DumpShort);
        p(bool, IsInDump){return isInDump;}
        p(std::string, DumpLong);
        static bool EnableSetDumpString;
        static bool EnableSetDumpStringAsync;
        static void SetDumpStringQueueEntryWait();
    private:
        std::string SetDumpStringWorker() const;
    };
};


template<typename TBase, typename TRealm>
int HWLib::WithId<TBase, TRealm>::NextObjectId = 0;

