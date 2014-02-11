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

    dumpString = Dump.RawData;
    dumpShortString = DumpShort.RawData;
}

p_implementation(DumpableObjectBase, String, DumpShort){
    return Dump;
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
    auto typeName = HWLib::TypeName(*this);
    auto objectId = DumpToString(ObjectId);
    return typeName + ".Id" + objectId;
}

p_implementation(DumpableObject, String, Dump){
    auto result = DumpHeader;
    Array<String> dataResult { "..." };

    if (!isInDump)
    {
        LevelValue<bool> inDump (isInDump, true);
        dataResult = DumpData;
    }
    
    return DumpHeader + String::Surround("{", dataResult, "}");
}

p_implementation(DumpableObject, String, DumpShort){
    return DumpHeader;
}

#include "TemplateInstances.h"
