#pragma once

#include "Properties.h"
#include <string>
#include <thread>

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
        mutable bool isInDump;

        friend class SetDumpStringQueueEntry;
        SetDumpStringQueueEntry* SetDumpStringToDo;
    protected:
        DumpableObject();                     
        DumpableObject(DumpableObject const&);
        virtual ~DumpableObject();
        virtual_p(Array<String>, DumpData) = 0;
        virtual_p(String, DumpHeader);
    public:
        void SetDumpString();
        p(String, Dump);
        virtual_p(String, DumpShort);
        p(bool, IsInDump){return isInDump;}
        p(String, DumpLong);
        static bool EnableSetDumpString;
        static bool EnableSetDumpStringAsync;
        static void SetDumpStringQueueEntryWait();
    private:
        void SetDumpStringWorker();
    };
};


template<typename TBase, typename TRealm>
int HWLib::WithId<TBase, TRealm>::NextObjectId = 0;

#undef __asm_int3
#define __asm_int3 {DumpableObject::SetDumpStringQueueEntryWait(); __asm{ int 3 }}
