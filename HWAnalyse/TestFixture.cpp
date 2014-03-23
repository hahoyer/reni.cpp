#include "Import.h"
#include "TestFixture.h"

static bool Trace = true;

using namespace HWAnalyse;
using namespace HWLib;


void TestFixture::RunAll(){
    auto all = RuntimeChain<TestFixture>::All()
        ->Sort(HasHigherPriority)
        ->ToArray;
    for(int i = 0; i < all.Count; i++){
        currentTest = all[i];
        auto name = typeid(*currentTest).name();
        _console_ WriteLine(currentTest->location());
        _console_ IndentLevel++;
        try{
            currentTest->Run();
        }
        catch(TestFailedException const&exception){
            _console_ IndentLevel--;
            _console_ WriteLine(String("test_(") + name + ") exception: ");
            _console_ IndentLevel++;
            _console_ WriteLine(HWLib::Dump(exception));
        }
        catch(...){
            _console_ IndentLevel--;
            _console_ WriteLine(String("test_(") + name + ") unexpected exception. Execution aborted.");
            throw;
        }
        _console_ IndentLevel--;
    }
};

bool TestFixture::HasHigherPriority(base const* left, base const* right){
    if(left->isLowPriority != right->isLowPriority)
        return right->isLowPriority;
    a_if_(left->dependencies.Count == 0);
    String leftName = typeid(*left).name();
    String rightName = typeid(*right).name();
    return leftName < rightName;
};

TestFixture::base* TestFixture::currentTest = {};
