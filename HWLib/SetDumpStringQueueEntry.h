#pragma once
#include "DumpableObject.h"

#include <mutex>

namespace HWLib{
    class SetDumpStringQueueEntry{
        static SetDumpStringQueueEntry rootInstance;
        static SetDumpStringQueueEntry* root;
        static std::thread worker;
        static std::recursive_mutex syncer;
        static int nextId;

        SetDumpStringQueueEntry* previous;
        SetDumpStringQueueEntry* next;
        DumpableObject const* entry;
        int const id;

        SetDumpStringQueueEntry()
            : id(nextId++)
            , entry({})
            , previous(this)
            , next(this){
            worker = std::thread(Run);

        }

        SetDumpStringQueueEntry(DumpableObject const&entry)
            : id(nextId++)
            , entry(&entry)
            , previous(root->previous)
            , next(root){
            assert(root == &rootInstance);
            next->previous = this;
            previous->next = this;
            }

        ~SetDumpStringQueueEntry() {
            a_if_(root == &rootInstance);
            a_if_(entry);
            a_if_(!entry->SetDumpStringToDo);
            //static int i = 0;
            //_console_.WriteLine(Dump(i++));
            previous->next = next;
            next->previous = previous;
            entry = {};
            //b_if_(i == 14);
        }

        static void Run(){
            while(true){
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                auto i = 0;
                std::unique_lock<std::recursive_mutex> l(syncer);
                for(; i < 1000 && root->previous != root; i++)
                    root->previous->Apply();
                //_console_.WriteLine(__FUNCTION__ " " + Dump(i));
            }
        };

        void Apply()
        {
            auto entry = const_cast<DumpableObject *>(this->entry);
            RemoveWorker(entry->SetDumpStringToDo);
            entry->SetDumpStringWorker();
        }

    public:
        static void Remove(SetDumpStringQueueEntry* entry){
            std::unique_lock<std::recursive_mutex> l(syncer);
            if(entry)
                RemoveWorker(entry);
        };

        static void RemoveWorker(SetDumpStringQueueEntry* entry){
            const_cast<DumpableObject *>(entry->entry)->SetDumpStringToDo = {};
            delete entry;
        };

        static SetDumpStringQueueEntry * Insert(DumpableObject const&entry){
            std::unique_lock<std::recursive_mutex> l(syncer);
            if(entry.SetDumpStringToDo)
                return entry.SetDumpStringToDo;

            if(DumpableObject::EnableSetDumpString){
                const_cast<DumpableObject &>(entry).SetDumpStringWorker();
                return{};
            }

            return new SetDumpStringQueueEntry(entry);
        };

        static void Wait(){
            auto i = 0;
            std::unique_lock<std::recursive_mutex> l(syncer);
            for(; root->previous != root; i++)
                root->previous->Apply();
            //_console_.WriteLine(__FUNCTION__ " " + Dump(i));
        }
    };
}


using namespace HWLib;

SetDumpStringQueueEntry SetDumpStringQueueEntry::rootInstance;
SetDumpStringQueueEntry* SetDumpStringQueueEntry::root = &rootInstance;
std::thread SetDumpStringQueueEntry::worker;
std::recursive_mutex SetDumpStringQueueEntry::syncer;
int SetDumpStringQueueEntry::nextId = 0;