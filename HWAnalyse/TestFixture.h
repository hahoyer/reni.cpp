#pragma once
#include "../HWLib/String.h"
#include "../HWLib/RuntimeChain.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/DumpToString.h"

using namespace HWLib;


namespace HWAnalyse{
    struct TestFailedException : public DumpableObject{
        virtual~TestFailedException(){}
    };

    struct TestFixture{
        struct base{
            Array<CtrlRef<base>> dependencies;
            bool isLowPriority;
            bool isSuccessful;
            bool isStarted;
            base(Array<CtrlRef<base>> const& dependencies)
                : isLowPriority(false)
                , isSuccessful(false)
                , isStarted(false)
                , dependencies(dependencies){
            };
            virtual void Run()const = 0;
            virtual String const location()const = 0;
            bool CheckedRun(bool skipLowPriority);
        private:
            void WatchedRun();
        };

        typedef base* dataType;
        static base* currentTest;

        static void RunAll();
    private:
        static bool RunAny(Array<base*> const&all, bool skipLowPriority);
    };
};
