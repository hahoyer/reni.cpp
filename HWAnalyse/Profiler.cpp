#include "Import.h"
#include "Profiler.h"

#include "Profiler.include.h"
#include <algorithm>

using namespace HWAnalysis;
using namespace std;


__int64 Profiler::internal::SuspendTime = 0;
__int64 Profiler::internal::ResumeTime = 0;

void Profiler::internal::Start(const char* FileName, int Line, const char* Flag)
{
  if(Top)
    Top->Suspend(SuspendTime);

  Entry* entry = Find(FileName, Flag);
  if(entry == nullptr)
  {
    entry = new Entry(FileName, Line, Flag);
    Entries.push_back(entry);
  };

  Stack.push_back(entry);
  Top->counter++;
  Top->Resume(ResumeTime);
};

void Profiler::internal::End()
{
  const auto top = Top;
  if(top == nullptr)
    return;
  top->Suspend();
  Stack.pop_back();
  if(Top)
    Top->Resume();
};

void Profiler::internal::Reset()
{
  while(Top)
    End();
  for(const auto entry : Entries)
    delete entry;
};

HW_PR_IMPL_GETTER(Profiler::internal, __int64, ticks)
{
  __int64 result = 0;
  for(const auto entry : Entries)
    result += entry->ticks;
  return result;
}

void Profiler::internal::CalibrationValues()
{
  for(auto counter = 0; counter < 1000; counter++)
  {
    Start("***Calibrate***", -1, "suspend time");
    Start("***Calibrate***", -1, "resume time");
    End();
    End();
  };

  for(const auto entry : Entries)
    entry->CalibrationValues(SuspendTime, ResumeTime);
  SuspendTime -= ResumeTime;
}

Profiler::internal::Entry* Profiler::internal::Find(const char* FileName, const char* Flag) const
{
  for(const auto entry : Entries)
    if(strcmp(FileName, entry->FileName) == 0 && Flag == entry->Flag)
      return entry;
  return {};
}

void Profiler::internal::dump(int Hide) const
{
  if(isEmpty)
  {
    ::dump("\n\r=========== Profile empty ============\n\r");
    return;
  }

  ::dump("\n\r=========== Profile ==================\n\r");

  auto entries = Entries;
  ranges::sort(entries, Entry::PointerLessThan());

  const auto All = ticks;
  auto ShowTime = All;
  auto TimeNotShown = All;
  auto ShowCount = entries.size();
  if(Hide > 0)
    ShowTime -= All / Hide;
  else if(-Hide > 0 && -Hide < entries.size())
    ShowCount = -Hide;
  size_t LFileName = 0;

  size_t i = 0;
  for(; i < ShowCount && ShowTime > 0; i++)
  {
    const auto That = entries[i];
    __int64 ticks = That->ticks;
    if(ticks < 0) ticks = 0;
    ShowTime -= ticks;
    TimeNotShown -= ticks;
    const size_t iLFileName = strlen(That->FileName);

    if(LFileName < iLFileName)
      LFileName = iLFileName;
  };
  ShowCount = i;

  for(i = 0; i < ShowCount; i++)
  {
    const auto That = entries[i];
    __int64 ticks = That->ticks;
    if(ticks < 0) ticks = 0;
    ::dump(That->FileName);
    ::dump("(");
    const size_t LLine = ::dump(That->Line);
    ::dump("):");

    size_t Spaces = LFileName + 6 - strlen(That->FileName) - LLine;
    while(Spaces > 0)
    {
      ::dump(" ");
      --Spaces;
    };

    ::dump(i);
    ::dump(":\t");
    size_t counter = ::dump(That->counter);
    ::dump("x");
    while(counter++ < 6) ::dump(" ");
    ::dump("\t");
    dumpTime(ticks, That->counter);
    ::dump(" \t");
    dumpTime(ticks, 1);
    ::dump(" \t");
    ::dump(static_cast<long long>(100.0 * static_cast<float>(ticks) / static_cast<float>(All)));
    ::dump("%\t");
    ::dump(That->Flag);
    ::dump("\n\r");
  };

  ::dump("Total: \t");
  dumpTime(All, 1);

  if(i < entries.size())
  {
    ::dump(" (");
    ::dump(entries.size() - i);
    ::dump(" not-shown-items ");
    dumpTime(TimeNotShown, 1);
    ::dump(")");
  };

  ::dump("\n\r======================================\n\r");
};


Profiler::Profiler()
  : _internal(*new internal)
{
  _internal.CalibrationValues();
};

Profiler::~Profiler() { delete &_internal; }

void Profiler::Start(const char* FileName, int Line, const char* Flag) const
{
  return _internal.Start(FileName, Line, Flag);
}

void Profiler::End() const { return _internal.End(); }
void Profiler::Reset() const { return _internal.Reset(); }
void Profiler::dump(int Hide) const { _internal.dump(Hide); }


void Profiler::internal::Entry::CalibrationValues(long long& suspend, long long& resume) const
{
  switch(*Flag)
  {
  case 's':
    suspend = ticks / counter;
    break;
  case 'r':
    resume = ticks / counter;
    break;
  default:
    break;
  };
};
