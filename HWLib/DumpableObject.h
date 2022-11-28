#pragma once

#include "BreakHandling.h"
#include "Properties.h"
#include <string>
#include <thread>

namespace HWLib
{
  template <typename T>
  class Array;


  template <typename TBase, typename TRealm = TBase>
  class WithId : public TBase
  {
    using baseType = TBase;
    using thisType = WithId;

  public:
    static int NextObjectId;
    int const ObjectId;
    WithId() : ObjectId(NextObjectId++) {};
    HW_PR_DECL_GETTER(std::string, DumpHeader) override;
  };


  class DumpableObject
  {
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
    HW_PR_VIRTUAL_GET(Array<std::string>, DumpData) = 0;
    HW_PR_VIRTUAL_GET(std::string, DumpHeader);

  public:
    void SetDumpString();
    HW_PR_GET(std::string, Dump);
    HW_PR_VIRTUAL_GET(std::string, DumpShort);
    HW_PR_GET(bool, IsInDump) { return isInDump; }
    HW_PR_GET(std::string, DumpLong);
    static bool EnableSetDumpString;
    static bool EnableSetDumpStringAsync;
    static void SetDumpStringQueueEntryWait();

  private:
    std::string SetDumpStringWorker() const;
  };
};


template <typename TBase, typename TRealm>
int WithId<TBase, TRealm>::NextObjectId = 0;
