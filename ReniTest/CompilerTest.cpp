#include "Import.h"
#include <type_traits>
#include "CompilerTest.h"

using namespace Reni;
using namespace ReniTest;

static bool Trace = true;

template <typename TResult, typename TTarget>
class ExtensionMethod
{
  using ImplementationType = function<TResult(const TTarget& target)>;

public:
  const ImplementationType Implementation;

  ExtensionMethod(ImplementationType implementation)
    : Implementation(implementation)
  { }
};

void CompilerTest::Check(const string& text, const string& expectedResult)
{
  CompilerTest ct = text;
  Compiler& co = ct.compiler;
  const auto result = co.Execute();
  if(result.data == expectedResult)
    return;

  HW_D_LOG("Text:\n" + text +"\n");
  HW_D_LOG_VALUE(expectedResult);
  HW_D_LOG_VALUE(result.data);
  HW_D_LOG("c++ code:\n" + co.cppCode + "\n");


  HW_FAIL_AND_THROW_;
};
