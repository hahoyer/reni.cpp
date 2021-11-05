#pragma once

#include "../HWAnalyse/TestMacros.h"
#include "../HWAnalyse/TestFixture.h"

namespace _Reni
{
  HW_TM_CLASS(ParseSimple);

  HW_TM_CLASS(CodeSimple);

  HW_TM_CLASS(CompileSimple);

  HW_TM_CLASS(Simple);

  HW_TM_CLASS(Negative);

  HW_TM_CLASS(SimpleFunction);

  HW_TM_CLASS(Function);

  HW_TM_CLASS(SimpleFunctionWithNonLocal);

  HW_TM_CLASS(RecursiveFunction);

  struct HW_TM_CLASS_NAME(PrimitiveRecursiveFunctionByteWithDump) final : public HWAnalysis::TestFixture::Data
  {
    HW_TM_CLASS_NAME(PrimitiveRecursiveFunctionByteWithDump)();
    string location() const override;
    void Run() const override;
    static
    RuntimeChain<HWAnalysis::TestFixture> instance;
  };
}
