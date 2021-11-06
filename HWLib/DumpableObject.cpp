#include "Import.h"
#include "DumpableObject.h"

#include "DumpToString.h"
#include "LevelValue.h"
#include "String.h"
#include "CtrlRef.h"
#include "SetDumpStringQueueEntry.h"
#include <thread>

using namespace HWLib;
using namespace std;

DumpableObject::DumpableObject() 
: isDumpStringValid(false)
, dumpString("")
, dumpShortString("")
, isInDump(false)
, SetDumpStringToDo(nullptr)
{};

DumpableObject::DumpableObject(DumpableObject const&other)
: isDumpStringValid(other.isDumpStringValid)
, dumpString(other.dumpString)
, dumpShortString(other.dumpShortString)
, isInDump(false)
, SetDumpStringToDo(nullptr)
{};


DumpableObject::~DumpableObject(){
    SetDumpStringQueueEntry::Remove(SetDumpStringToDo);
}

bool DumpableObject::EnableSetDumpString = false;
bool DumpableObject::EnableSetDumpStringAsync = true;

void DumpableObject::SetDumpString(){
    if(!HW_CONSOLE.IsDebuggerPresent)
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

string DumpableObject::SetDumpStringWorker() const
{
    dumpString = DumpLong.c_str();
    dumpShortString = DumpShort.c_str();
    isDumpStringValid = true;
    return dumpString;
}

p_implementation(DumpableObject, string, Dump){
    return DumpLong;
}

p_virtual_header_implementation(DumpableObject, string, DumpHeader);

p_implementation(DumpableObject, string, DumpHeader){return HWLib::DumpTypeName(*this);};

p_implementation(DumpableObject, string, DumpLong){
    auto result = DumpHeader;
    Array<string> dataResult { "..." };

    if (!isInDump)
    {
        LevelValue<bool> inDump (isInDump, true);
        dataResult = DumpData;
    }
    
    return DumpHeader + String::Surround("{", dataResult, "}");
};

p_virtual_implementation(DumpableObject, string, DumpShort){
    return DumpHeader;
};

p_virtual_header_implementation(DumpableObject, Array<string>, DumpData);

#include "TemplateInstances.h"
