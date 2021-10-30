#pragma once
#include "../HWLib/String.h"
#include "../HWLib/RuntimeChain.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/DumpToString.h"

using namespace HWLib;


namespace HWAnalyse
{
    struct TestFailedException : public DumpableObject
    {
        virtual ~TestFailedException()
        {
        }
    };

    struct TestFixture
    {
        struct Data
        {
            Array<CtrlRef<Data>> dependencies;
            bool isLowPriority;
            bool isSuccessful;
            bool isStarted;

            Data(Array<CtrlRef<Data>> const& dependencies)
                : isLowPriority(false)
                  , isSuccessful(false)
                  , isStarted(false)
                  , dependencies(dependencies)
            {
            };

            virtual ~Data() = default;

            virtual void Run()const = 0;
            virtual String const location()const = 0;
            bool CheckedRun(bool skipLowPriority);
            void WatchedRun();
        };

        using dataType = Data*; //Used for runtime chain
        static Data* currentTest;

        static void RunAll();
    private:
        static bool RunAny(Array<Data*> const& all, bool skipLowPriority);
    };
};