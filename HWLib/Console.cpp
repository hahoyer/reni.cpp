#include "Import.h"
#include "Console.h"

#include "Common.h"
#include "DumpableObject.h"
#include "LevelValue.h"
#include "String.h"

using namespace HWLib;
using namespace std;

Console Console::Instance;

Console::Console()
  : _isLineStart(true)
    , IndentCount(0)
{}

HW_PR_IMPL_GETTER(Console, bool, IsDebuggerPresent) { return !!::IsDebuggerPresent(); };
HW_PR_IMPL_MUTABLE_GETTER(Console, LevelValue<int>, IndentLevel) { return IndentCount; };

static void OutputDebug(const String& text)
{
  if(IsDebuggerPresent())
    OutputDebugStringA(text.RawData);
  else
    printf("%s", text.RawData);
}

void Console::Write(const string& text, bool isLine) const
{
  const auto formattedText = text | Indent(_isLineStart, IndentCount);
  OutputDebug(formattedText + (isLine ? string("\n") : string("")));
}

bool Console::BreakTrace(const string& condition, const string& fileName, int line, const string& data) const
{
  WriteLine(fileName | FilePosition(line, 0, condition));
  if(data != "")
    WriteLine("Data: " + data);

  DumpableObject::SetDumpStringQueueEntryWait();
  return true;
}
