#include "Import.h"
#include "Console.h"

#include "Common.h"
#include "File.h"
#include "String.h"
#include "DumpableObject.h"
#include "LevelValue.h"

using namespace HWLib;

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

void Console::Write(String const&text, bool isLine)
{
    auto formattedText = text.Indent(_isLineStart, IndentCount);
    OutputDebug(formattedText + (isLine ? String("\n") : String("")));
}

bool Console::BreakTrace(const String&condition, const String&fileName, int line, const String&data)
{
    WriteLine(String::FilePosition(fileName, line, 0, condition));
    if (data != "")
        WriteLine(String("Data: ") + data);

    DumpableObject::SetDumpStringQueueEntryWait(); 
    return true;
}

