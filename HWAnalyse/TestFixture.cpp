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

  c_.WriteLine("\n--- RUnning low priority tests ---");
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
  c_.WriteLine(location());
  auto indentLevel = c_.IndentLevel;
  c_.IndentCount++;
  try
  {
    Run();
     isSuccessful = true;
  }
  catch(const TestFailedException& exception)
  {
    c_.IndentCount--;
    c_.WriteLine(string("test_(") + name + ") exception: ");
    c_.IndentCount++;
    c_.WriteLine(Dump(exception));
  }
  catch(...)
  {
    c_.IndentCount--;
    c_.WriteLine(string("test_(") + name + ") unexpected exception. Execution aborted.");
    throw;
  }
};

TestFixture::Data* TestFixture::currentTest = {};
