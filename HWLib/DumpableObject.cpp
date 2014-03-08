#include "Import.h"
#include "DumpableObject.h"

#include "DumpToString.h"
#include "LevelValue.h"
#include "String.h"
#include "CtrlRef.h"

using namespace HWLib;

DumpableObject::DumpableObject() 
: isInDump(false)
, dumpString("missing call to SetDumpString()")
, dumpShortString("missing call to SetDumpString()")
{};

void DumpableObject::SetDumpString()
{
    if(!_console_ IsDebuggerPresent)
        return;

    dumpString = DumpLong.RawData;
    dumpShortString = DumpShort.RawData;
}

p_implementation(DumpableObject, String, Dump){
    return DumpLong;
}

pure_p_implementation(DumpableObject, String, DumpHeader);
override_p_implementation(DumpableObject, String, DumpHeader){
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
