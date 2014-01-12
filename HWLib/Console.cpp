#include "Import.h"
#include "Console.h"

#include <windows.h>
#include "Common.h"
#include "File.h"
#include "String.h"

using namespace HWLib;

Console Console::Instance;

Console::Console()
: _isLineStart(true)
, IndentLevel(0)
{
}

static void OutputDebug(const String&text)
{
    if (::IsDebuggerPresent())
        ::OutputDebugStringA(text.ToArray.RawData);
    else
        ::printf("%s", text.ToArray.RawData);
}

void Console::Write(String const&text, bool isLine)
{
    auto formattedText = text.Indent(_isLineStart, IndentLevel);
    OutputDebug(formattedText + (isLine ? String("\n") : String("")));
}

bool Console::BreakTrace(const String&condition, const String&fileName, int line, const String&data)
{
    WriteLine(String::FilePosition(fileName, line, 0, condition));
    if (data != "")
        WriteLine(String("Data: ") + data);
    return true;
}
