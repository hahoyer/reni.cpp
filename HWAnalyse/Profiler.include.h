#pragma once

#include "Profiler.h"
#include "Profiler.util.h"
#include "../HWLib/Properties.h"
#include <vector>


using namespace HWAnalysis;
using namespace std;

class Profiler::internal
{
  static __int64 SuspendTime;
  static __int64 ResumeTime;
  class Entry;
  vector<Entry*> Entries{};
  vector<Entry*> Stack{};
public:
  void Start(const char* FileName, int Line, const char* Flag);
  void End();
  void dump(int Hide = 10) const;
  void CalibrationValues();
  void Reset();
private:
  p(__int64, ticks);
  p(bool, isEmpty) { return Entries.size() == 0; };
  p(bool, isAtTopLevel) { return Stack.size() == 0; };
  p(Entry*, Top) { return !isAtTopLevel ? Stack.back() : nullptr; }
  Entry* Find(const char* FileName, const char* Flag) const;
};


class Profiler::internal::Entry
{
public:
  struct PointerLessThan;

  const char* const FileName;
  const int Line;
  const char* const Flag;

  __int64 lastTicks{};
  __int64 ticks;
  __int64 counter;

  Entry(const char* fn, int l, const char* fl)
    : FileName(fn), Line(l), Flag(fl), ticks(0), counter(0) { };

  void Suspend() { ticks += System::Ticks() - lastTicks; };
  void Resume() { lastTicks = System::Ticks(); };
  void Suspend(__int64 Correction) { ticks += System::Ticks() - lastTicks - Correction; };
  void Resume(__int64 Correction) { lastTicks = System::Ticks() + Correction; };
  void CalibrationValues(__int64& suspend, __int64& resume) const;
};

struct Profiler::internal::Entry::PointerLessThan
{
  bool operator()(const Entry* left, const Entry* right) const { return left->ticks > right->ticks; }
};
