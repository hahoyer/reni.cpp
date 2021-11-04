#include "Import.h"
#include "Console.h"

#include "Common.h"
#include "File.h"
#include "String.h"
#include "DumpableObject.h"
#include "LevelValue.h"

using namespace HWLib;
using namespace std;

Console Console::Instance;

Console::Console()
: _isLineStart(true)
, IndentCount(0)
{
}

p_implementation(Console, bool, IsDebuggerPresent){return !!::IsDebuggerPresent();};
p_nonconst_implementation(Console, LevelValue<int>, IndentLevel){return IndentCount;};

static void OutputDebug(const String&text)
{
    if (::IsDebuggerPresent())
        ::OutputDebugStringA(text.RawData);
    else
        ::printf("%s", text.RawData);
}

void Console::Write(string const&text, bool isLine)
{
    auto formattedText = text|Indent(_isLineStart, IndentCount);
    OutputDebug(formattedText + (isLine ? string("\n") : string("")));
}

bool Console::BreakTrace(const string&condition, const string&fileName, int line, const string&data)
{
  WriteLine(fileName|FilePosition(line, 0, condition));
    if (data != "")
        WriteLine("Data: " + data);

    DumpableObject::SetDumpStringQueueEntryWait(); 
    return true;
}

