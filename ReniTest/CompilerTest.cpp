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

void CompilerTest::Check(const String& text, const String expectedResult)
{
  CompilerTest ct = text;
  Compiler& co = ct.compiler;
  const auto result = co.Execute();
  if(result.data == expectedResult)
    return;

  dd("Text:\n" + text +"\n");
  d(expectedResult);
  d(result.data);
  dd("c++ code:\n" + co.cppCode + "\n");


  a_fail_throw_;
};
