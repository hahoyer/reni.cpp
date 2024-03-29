#pragma once
#include "DumpableObject.h"

#include <mutex>
#include "System.h"

namespace HWLib
{
  class SetDumpStringQueueEntry
  {
    static SetDumpStringQueueEntry rootInstance;
    static SetDumpStringQueueEntry* root;
    static std::thread worker;
    static std::recursive_mutex syncer;
    static int nextId;

    SetDumpStringQueueEntry* previous;
    SetDumpStringQueueEntry* next;
    const DumpableObject* entry;
    const int id;

    SetDumpStringQueueEntry()
      : previous(this)
        , next(this)
        , entry(nullptr)
        , id(nextId++)
    {
      // worker = std::thread(Run);
    }

    SetDumpStringQueueEntry(const DumpableObject& entry)
      : previous(root->previous)
        , next(root)
        , entry(&entry)
        , id(nextId++)
    {
      HW_ASSERT_(root == &rootInstance);
      next->previous = this;
      previous->next = this;
    }

    ~SetDumpStringQueueEntry()
    {
      HW_ASSERT_(root == &rootInstance);
      HW_ASSERT_(entry);
      HW_ASSERT_(!entry->SetDumpStringToDo);
      //static int i = 0;
      //HW_CONSOLE.WriteLine(Dump(i++));
      previous->next = next;
      next->previous = previous;
      entry = {};
      //HW_BREAK_IF_(i == 14);
    }

    static void Run()
    {
      while(true)
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        auto i = 0;
        std::unique_lock<std::recursive_mutex> l(syncer);
        for(; i < 1000 && root->previous != root; i++)
          root->previous->Apply();
        //HW_CONSOLE.WriteLine(__FUNCTION__ " " + Dump(i));
      }
    };

    const std::string Apply() const
    {
      const auto entry = const_cast<DumpableObject*>(this->entry);
      RemoveWorker(entry->SetDumpStringToDo);
      return entry->SetDumpStringWorker();
    }

  public:
    static void Remove(SetDumpStringQueueEntry* entry)
    {
      std::unique_lock<std::recursive_mutex> l(syncer);
      if(entry)
        RemoveWorker(entry);
    };

    static void RemoveWorker(SetDumpStringQueueEntry* entry)
    {
      const_cast<DumpableObject*>(entry->entry)->SetDumpStringToDo = {};
      delete entry;
    };

    static SetDumpStringQueueEntry* Insert(const DumpableObject& entry)
    {
      std::unique_lock<std::recursive_mutex> l(syncer);
      if(entry.SetDumpStringToDo)
        return entry.SetDumpStringToDo;

      return new SetDumpStringQueueEntry(entry);
    };

    static void Wait()
    {
      auto i = 0;
      std::unique_lock<std::recursive_mutex> l(syncer);
      const auto tpms = System::TicksPerSecond() / 1000;
      for(; root->previous != root; i++)
      {
        const auto start = System::Ticks();
        auto result = root->previous->Apply();
        auto duration = (System::Ticks() - start) / tpms;
        if constexpr(false)
          HW_CONSOLE.WriteLine(
            __FUNCTION__ " "
            + Dump(i)
            + " "
            + String::Convert(duration)
            + " "
            + String::Convert(result.c_str())
          );
      }
    }
  };
}


using namespace HWLib;

SetDumpStringQueueEntry SetDumpStringQueueEntry::rootInstance;
SetDumpStringQueueEntry* SetDumpStringQueueEntry::root = &rootInstance;
std::thread SetDumpStringQueueEntry::worker;
std::recursive_mutex SetDumpStringQueueEntry::syncer;
int SetDumpStringQueueEntry::nextId = 0;
