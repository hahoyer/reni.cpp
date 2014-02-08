#include "Import.h"
#include "DumpableObject.h"

#include "String.h"
#include "Ref.h"

using namespace HWLib;

DumpableObject::DumpableObject()
: _dumpString(typeid(*this).name())
, _dumpShortString(typeid(*this).name())
{}


void DumpableObject::SetDumpString()
{
    if (_console_ IsDebuggerPresent)
        _dumpString = Dump.RawData;
}

p_implementation(DumpableObject, String, DumpShort){
    return Dump;
}

