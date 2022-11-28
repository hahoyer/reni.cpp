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

DumpableObject::DumpableObject(DumpableObject const& other)
  : isDumpStringValid(other.isDumpStringValid)
    , dumpString(other.dumpString)
    , dumpShortString(other.dumpShortString)
    , isInDump(false)
    , SetDumpStringToDo(nullptr)
{};


DumpableObject::~DumpableObject()
{
  SetDumpStringQueueEntry::Remove(SetDumpStringToDo);
}

bool DumpableObject::EnableSetDumpString = false;
bool DumpableObject::EnableSetDumpStringAsync = true;

void DumpableObject::SetDumpString()
{
  if(!HW_CONSOLE.IsDebuggerPresent)
    return;

  if(!EnableSetDumpString)
    return;

  if(EnableSetDumpStringAsync)
    SetDumpStringToDo = SetDumpStringQueueEntry::Insert(*this);
  else
    SetDumpStringWorker();
}

void DumpableObject::SetDumpStringQueueEntryWait()
{
  SetDumpStringQueueEntry::Wait();
}

string DumpableObject::SetDumpStringWorker() const
{
  dumpString = DumpLong.c_str();
  dumpShortString = DumpShort.c_str();
  isDumpStringValid = true;
  return dumpString;
}

HW_PR_IMPL_GETTER(DumpableObject, string, Dump)
{
  return DumpLong;
}

HW_PR_VIRTUAL_GETTER_WRAPPER(DumpableObject, string, DumpHeader);
HW_PR_IMPL_GETTER(DumpableObject, string, DumpHeader) { return DumpTypeName(*this); };

HW_PR_IMPL_GETTER(DumpableObject, string, DumpLong)
{
  auto result = DumpHeader;
  Array<string> dataResult{"..."};

  if(!isInDump)
  {
    LevelValue<bool> inDump(isInDump, true);
    dataResult = DumpData;
  }

  return DumpHeader + String::Surround("{", dataResult, "}");
};

HW_PR_VIRTUAL_GETTER_WRAPPER(DumpableObject, string, DumpShort);
HW_PR_IMPL_GETTER(DumpableObject, string, DumpShort) { return DumpHeader; };

HW_PR_VIRTUAL_GETTER_WRAPPER(DumpableObject, Array<string>, DumpData);

#include "TemplateInstances.h"
