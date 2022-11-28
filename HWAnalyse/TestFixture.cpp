#include "Import.h"
#include "TestFixture.h"

[[maybe_unused]] static bool Trace = true;

using namespace HWAnalysis;
using namespace HWLib;


void TestFixture::RunAll()
{
  const auto all = RuntimeChain<TestFixture>::All()
    ->ToArray;

  while(RunAny(all, true))
    continue;

  HW_CONSOLE.WriteLine("\n--- RUnning low priority tests ---");
  while(RunAny(all, false))
    continue;
};

bool TestFixture::RunAny(const Array<Data*>& all, bool skipLowPriority)
{
  auto result = false;
  for(int i = 0; i < all.Count; i++)
    result = all[i]->CheckedRun(skipLowPriority) || result;
  return result;
};

bool TestFixture::Data::CheckedRun(bool skipLowPriority)
{
  if(isStarted)
    return false;
  if(skipLowPriority && isLowPriority)
    return false;
  if(dependencies
     .Where([](CtrlRef<Data> dependency) { return !dependency->isSuccessful; })
     ->Any)
    return false;
  isStarted = true;
  currentTest = this;
  WatchedRun();
  return true;
};


void TestFixture::Data::WatchedRun()
{
  const auto name = typeid(*this).name();
  HW_CONSOLE.WriteLine(location());
  auto indentLevel = HW_CONSOLE.IndentLevel;
  HW_CONSOLE.IndentCount++;
  try
  {
    Run();
    isSuccessful = true;
  }
  catch(const TestFailedException& exception)
  {
    HW_CONSOLE.IndentCount--;
    HW_CONSOLE.WriteLine(string("test_(") + name + ") exception: ");
    HW_CONSOLE.IndentCount++;
    HW_CONSOLE.WriteLine(Dump(exception));
  }
  catch(...)
  {
    HW_CONSOLE.IndentCount--;
    HW_CONSOLE.WriteLine(string("test_(") + name + ") unexpected exception. Execution aborted.");
    throw;
  }
};

TestFixture::Data* TestFixture::currentTest = {};
