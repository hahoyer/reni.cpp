#include "Import.h"
#include "DumpableObject.h"

#include "DumpToString.h"
#include "LevelValue.h"
#include "String.h"
#include "CtrlRef.h"
#include "SetDumpStringQueueEntry.h"
#include <thread>

using namespace HWLib;

DumpableObject::DumpableObject() 
: isInDump(false)
, dumpString("missing call to SetDumpString()")
, dumpShortString("missing call to SetDumpString()")
, SetDumpStringToDo({})
{};

DumpableObject::DumpableObject(DumpableObject const&other)
: isInDump(false)
, dumpString(other.dumpString)
, dumpShortString(other.dumpShortString)
, SetDumpStringToDo({})
{};


DumpableObject::~DumpableObject(){
    SetDumpStringQueueEntry::Remove(SetDumpStringToDo);
}

bool DumpableObject::EnableSetDumpString = false;
bool DumpableObject::EnableSetDumpStringAsync = true;

void DumpableObject::SetDumpString(){
    if(!_console_.IsDebuggerPresent)
        return;
    
    if(!EnableSetDumpString)
        return;
    
    if(EnableSetDumpStringAsync)
        SetDumpStringToDo = SetDumpStringQueueEntry::Insert(*this);
    else
        SetDumpStringWorker();
}

void DumpableObject::SetDumpStringQueueEntryWait(){
    SetDumpStringQueueEntry::Wait();
}

void DumpableObject::SetDumpStringWorker(){
    dumpString = DumpLong.RawData;
    dumpShortString = DumpShort.RawData;
}

p_implementation(DumpableObject, String, Dump){
    return DumpLong;
}

pure_p_implementation(DumpableObject, String, DumpHeader);
p_implementation(DumpableObject, String, DumpHeader){
    return HWLib::DumpTypeName(*this);
};

p_implementation(DumpableObject, String, DumpLong){
    auto result = DumpHeader;
    Array<String> dataResult { "..." };

    if (!isInDump)
    {
        LevelValue<bool> inDump (isInDump, true);
        dataResult = DumpData;
    }
    
    return DumpHeader + String::Surround("{", dataResult, "}");
};

virtual_p_implementation(DumpableObject, String, DumpShort){
    return DumpHeader;
};

pure_p_implementation(DumpableObject, Array<String>, DumpData);

#include "TemplateInstances.h"
