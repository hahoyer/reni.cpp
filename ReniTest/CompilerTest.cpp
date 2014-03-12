#include "Import.h"
#include "CompilerTest.h"

using namespace Reni;
using namespace ReniTest;

static bool Trace = true;

void CompilerTest::Check(String const& text, String const expectedResult){
    CompilerTest ct = text;
    Compiler& co = ct.compiler;
    auto result = co.Execute();
    if(result.data == expectedResult)
        return;
    
    dd("Text:\n" + text +"\n");
    nd(expectedResult);
    nd(result.data);
    dd("c++:\n" + co.cppCode + "\n");
    a_fail_throw_;
};
