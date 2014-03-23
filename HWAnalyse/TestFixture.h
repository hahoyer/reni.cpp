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
            base(Array<CtrlRef<base>> const& dependencies)
                : isLowPriority(false)
                , dependencies(dependencies){
            };
            virtual void Run()const = 0;
            virtual String const location()const = 0;
        };

        typedef base* dataType;
        static base* currentTest;

        static void RunAll();
        static bool HasHigherPriority(base const* left, base const* right);
    };
};
