#pragma once

#include "Profiler.h"
#include "Profiler.util.h"
#include "../HWLib/Properties.h"
#include <vector>


using namespace HWAnalyse;
using namespace std;

class Profiler::internal{
    static __int64 SuspendTime;
    static __int64 ResumeTime;
    class Entry;
    vector<Entry*> Entries;
    vector<Entry*> Stack;
public:
    void Start(const char*FileName, int Line, const char*Flag);
    void End();
    void dumpprint(int Hide = 10)const;
    void CalibrationValues();
    void Reset();
private:
    p(int, count);
    p(__int64, ticks);
    p(bool, isEmpty){ return Entries.size() == 0; };
    p(bool, isAtTopLevel){ return Stack.size() == 0; };
    p(Entry*, Top){ return !isAtTopLevel ? Stack.back() : NULL; }
    Entry* const Find(const char*FileName, const char*Flag)const;
};


class Profiler::internal::Entry{
public:
    struct PointerLessThan;

    char const*const FileName;
    const int Line;
    const char *const Flag;

    __int64 lastTicks;
    __int64 ticks;
    __int64 counter;

    Entry(const char*fn, int l, const char*fl)
        : FileName(fn), Line(l), Flag(fl), ticks(0), counter(0){
    };

    void Suspend(){ ticks += ::Ticks() - lastTicks; };
    void Resume(){ lastTicks = ::Ticks(); };
    void Suspend(__int64 Correction){ ticks += ::Ticks() - lastTicks - Correction; };
    void Resume(__int64 Correction){ lastTicks = ::Ticks() + Correction; };
    void CalibrationValues(__int64&suspend, __int64&resume)const;
};

struct Profiler::internal::Entry::PointerLessThan{
    inline bool operator() (Entry const*left, Entry const*right){return left->ticks > right->ticks;}
};

