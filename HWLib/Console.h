#pragma once
#include "Properties.h"

namespace HWLib
{
  template <typename T>
  class LevelValue;

  class Console final
  {
    bool _isLineStart;
  public:
    int IndentCount;
    Console();
    void Write(const std::string& text, bool isLine = false) const;
    void WriteLine(const std::string& text) { Write(text, true); };
    bool BreakTrace(const std::string& condition, const std::string& fileName, int line, const std::string& data);
    template <typename T>
    void FunctionTrace(const std::string& function, const T& object);

    p_nonconst(LevelValue<int>, IndentLevel);
    p(bool, IsDebuggerPresent);
    static Console Instance;
  };

  template <typename T>
  std::string Dump(const T& object);
}

#define HW_CONSOLE ::HWLib::Console::Instance

using namespace HWLib;


template <typename T>
void Console::FunctionTrace(const std::string& function, const T& object)
{
  Write(function);
  if(typeid(object) != typeid(T))
  {
    auto header
      = " *** "
      + DumpTypeName(object)
      + " *** ";
    Write(header);
  }

  IndentCount++;
  Write("[ " + HWLib::Dump(object) + " ]\n");
};
