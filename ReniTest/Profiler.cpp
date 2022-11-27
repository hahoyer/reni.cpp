#include "Import.h"
#include "Profiler.h"

#include "../HWAnalyse/Profiler.h"

using namespace HWLang;
using namespace HWLib;

static bool Trace = true;

namespace TestProfiler
{
  auto SomeFunction(auto value)
  {
    return value + 1;
  }

  HW_TM_IMPLEMENTATION(MacroCallsNoProfiler)
  {
    HW_PROF_BEGIN("test");
    auto x = 3;
    HW_PROF_NEXT("test2");
    x++;
    HW_PROF_END();
    x++;

    auto y = HW_PROF_FUNCTION(SomeFunction(x));

    HW_PROF_STATEMENT(x++);
  }

  HWAnalysis::Profiler instance;

#define HW_PROF_INSTANCE instance

  HW_TM_IMPLEMENTATION(MacroCallsProfiler)
  {
    HW_PROF_BEGIN("test");
    auto x = 3;
    HW_PROF_NEXT("test2");
    x++;
    HW_PROF_END();
    x++;

    auto y = HW_PROF_FUNCTION(SomeFunction(x));

    HW_PROF_STATEMENT(x++);
  }
}
