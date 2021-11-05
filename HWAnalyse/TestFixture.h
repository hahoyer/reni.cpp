#pragma once
#include "../HWLib/String.h"
#include "../HWLib/RuntimeChain.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/DumpToString.h"
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/seq/enum.hpp>

using namespace HWLib;

namespace HWAnalysis
{
  struct TestFailedException : public DumpableObject
  {
    TestFailedException(TestFailedException const&) = delete;
    TestFailedException(TestFailedException &&) = delete;

    ~TestFailedException() override = default;

    TestFailedException& operator=(TestFailedException const&) = delete;
    TestFailedException& operator=(TestFailedException &&) = delete;
  };

  struct TestFixture
  {
    struct Data
    {
      HW_DO_DELETE(Data, CC,MC,CA,MA);

      Array<CtrlRef<Data>> dependencies;

      bool isLowPriority;
      bool isSuccessful;
      bool isStarted;

      explicit Data(const Array<CtrlRef<Data>>& dependencies)
        : isLowPriority(false)
          , isSuccessful(false)
          , isStarted(false)
          , dependencies(dependencies)
      { };

      virtual ~Data() = default;
      virtual void Run() const = 0;
      virtual std::string location() const = 0;
      bool CheckedRun(bool skipLowPriority);
      void WatchedRun();
    };

    using dataType = Data*; //Used for runtime chain
    static Data* currentTest;

    static void RunAll();
  private:
    static bool RunAny(const Array<Data*>& all, bool skipLowPriority);
  };
};
