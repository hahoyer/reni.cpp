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
    while(RunAny(all, false))
        continue;
};

bool TestFixture::RunAny(Array<base*> const&all, bool skipLowPriority){
    auto result = false;
    for(int i = 0; i < all.Count; i++)
        result = all[i]->CheckedRun(skipLowPriority) || result;
    return result;
};

bool TestFixture::base::CheckedRun(bool skipLowPriority){
    if(isStarted)
        return false;
    if(!skipLowPriority&& isLowPriority)
        return false;
    bool hasUnsuccessfulDependant 
        = dependencies
        .Where([](CtrlRef<base> dependant){return !dependant->isSuccessful;})
        ->Any;
    if(hasUnsuccessfulDependant)
        return false;
    isStarted = true;
    currentTest = this;
    WatchedRun();
    return true;
};

void TestFixture::base::WatchedRun(){
    auto name = typeid(*this).name();
    _console_ WriteLine(location());
    _console_ IndentLevel++;
    try{
        Run();
        isSuccessful = true;
    }
    catch(TestFailedException const&exception){
        _console_ IndentLevel--;
        _console_ WriteLine(String("test_(") + name + ") exception: ");
        _console_ IndentLevel++;
        _console_ WriteLine(Dump(exception));
    }
    catch(...){
        _console_ IndentLevel--;
        _console_ WriteLine(String("test_(") + name + ") unexpected exception. Execution aborted.");
        throw;
    }
    _console_ IndentLevel--;
};

TestFixture::base* TestFixture::currentTest = {};