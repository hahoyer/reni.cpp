#include "Import.h"
#include "DumpableObject.h"

#include "DumpToString.h"
#include "LevelValue.h"
#include "String.h"
#include "CtrlRef.h"

using namespace HWLib;

//////////////////////////////////////////////////////

DumpableObjectBase::DumpableObjectBase()
: dumpString(typeid(*this).name())
, dumpShortString(typeid(*this).name())
{}

void DumpableObjectBase::SetDumpString()
{
    if (!_console_ IsDebuggerPresent)
        return;

    dumpString = DumpLong.RawData;
    dumpShortString = DumpShort.RawData;
}

pure_p_implementation(DumpableObjectBase, String, DumpLong);

virtual_p_implementation(DumpableObjectBase, String, DumpShort){
    return DumpLong;
}

p_implementation(DumpableObjectBase, String, Dump){
    return DumpLong;
}

//////////////////////////////////////////////////////

DumpableObject::DumpableObject() 
: isInDump(false)
{};

pure_p_implementation(DumpableObject, String, DumpHeader);
override_p_implementation(DumpableObject, String, DumpHeader){
    return HWLib::DumpTypeName(*this);
};

override_p_implementation(DumpableObject, String, DumpLong){
    auto result = DumpHeader;
    Array<String> dataResult { "..." };

    if (!isInDump)
    {
        LevelValue<bool> inDump (isInDump, true);
        dataResult = DumpData;
    }
    
    return DumpHeader + String::Surround("{", dataResult, "}");
};

override_p_implementation(DumpableObject, String, DumpShort){
    return DumpHeader;
};

pure_p_implementation(DumpableObject, Array<String>, DumpData);

#include "TemplateInstances.h"
