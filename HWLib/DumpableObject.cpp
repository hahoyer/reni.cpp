#include "Import.h"
#include "DumpableObject.h"

#include "DumpToString.h"
#include "LevelValue.h"
#include "String.h"
#include "Ref.h"

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

p_implementation(DumpableObjectBase, String, DumpShort){
    return DumpLong;
}

p_implementation(DumpableObjectBase, String, Dump){
    return DumpLong;
}

//////////////////////////////////////////////////////

static int nextObjectId = 0;

DumpableObject::DumpableObject()
: DumpableObject(nextObjectId++)
{};


DumpableObject::DumpableObject(int objectId) 
: ObjectId(objectId++)
, isInDump(false)
{};

p_implementation(DumpableObject, String, DumpHeader){
    auto typeName = HWLib::DumpTypeName(*this);
    auto objectId = HWLib::Dump(ObjectId);
    return typeName + ".Id" + objectId;
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

p_implementation(DumpableObject, String, DumpShort){
    return DumpHeader;
};

#include "TemplateInstances.h"
