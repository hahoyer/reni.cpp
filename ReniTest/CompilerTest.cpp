#include "Import.h"
#include "CompilerTest.h"

using namespace Reni;
using namespace ReniTest;

static bool Trace = true;

void CompilerTest::Check(String const& text, String const expectedResult){
    CompilerTest ct = text;
    auto result = ct.compiler.Execute();
    a_is(result.data, == , expectedResult);
};
