#include "Import.h"
#include "TestFixture.h"

static bool Trace = true;

using namespace HWAnalyse;
using namespace HWLib;


void TestFixture::RunAll(){
    auto all = RuntimeChain<TestFixture>::All()
        ->ToArray;

    while(RunAny(all, true))
        continue;

    c_.WriteLine("\n--- RUnning low priority tests ---");
    while(RunAny(all, false))
        continue;
};

bool TestFixture::RunAny(Array<Data*> const&all, bool skipLowPriority){
    auto result = false;
    for(int i = 0; i < all.Count; i++)
        result = all[i]->CheckedRun(skipLowPriority) || result;
    return result;
};

bool TestFixture::Data::CheckedRun(bool skipLowPriority){
    if(isStarted)
        return false;
    if(skipLowPriority&& isLowPriority)
        return false;
    bool hasUnsuccessfulDependant 
        = dependencies
        .Where([](CtrlRef<Data> dependant){return !dependant->isSuccessful;})
        ->Any;
    if(hasUnsuccessfulDependant)
        return false;
    isStarted = true;
    currentTest = this;
    WatchedRun();
    return true;
};

void TestFixture::Data::WatchedRun(){
    auto name = typeid(*this).name();
    c_.WriteLine(location());
    c_.IndentLevel++;
    try{
        Run();
        isSuccessful = true;
    }
    catch(TestFailedException const&exception){
        c_.IndentLevel--;
        c_.WriteLine(String("test_(") + name + ") exception: ");
        c_.IndentLevel++;
        c_.WriteLine(Dump(exception));
    }
    catch(...){
        c_.IndentLevel--;
        c_.WriteLine(String("test_(") + name + ") unexpected exception. Execution aborted.");
        throw;
    }
    c_.IndentLevel--;
};

TestFixture::Data* TestFixture::currentTest = {};