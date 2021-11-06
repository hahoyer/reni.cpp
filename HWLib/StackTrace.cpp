#include "Import.h"
#include "StackTrace.h"

#include "Array.h"
#include "String.h"
#include "DumpMacros.h"
#include "DumpToString.h"
#include "TemplateInstances.h"
#include "System.h"

#pragma comment(lib, "DbgHelp.lib")

using namespace HWLib;
using namespace std;

static bool Trace = true;

#define WinApiCall(call) a_if(call, System::FormatLastErrorMessage())


StackFrameData::StackFrameData(DWORD64 instructionPointer)
  : instructionPointer(instructionPointer)
{
  IMAGEHLP_LINE64 lineInfo = {sizeof(IMAGEHLP_LINE64)};
  DWORD _lineOffset;
  const auto rc = SymGetLineFromAddr64(GetCurrentProcess(), instructionPointer, &_lineOffset, &lineInfo);
  if(rc)
  {
    fileName = lineInfo.FileName;
    lineNumber = static_cast<int>(lineInfo.LineNumber);
    lineOffset = _lineOffset;
    moduleName = System::ModuleName(instructionPointer);
  }
  else if(GetLastError() != 487)
    errorMessage = System::FormatLastErrorMessage();
};

string StackFrameData::Format() const
{
  string Return;
  if(fileName != "")
    Return += fileName | FilePosition(lineNumber, 0);
  Return += moduleName;
  Return += " @ 0x" + Dump(instructionPointer, 16);
  if(fileName != "")
    Return += " Offset in Line: 0x" + Dump(lineOffset, 16);
  if(errorMessage != "")
    Return += "error: " + errorMessage;
  return Return;
};


void ShowSymGetOptions()
{
  auto flags = SymGetOptions();
  String result;
#define ShowSymGetOptions_CONVERT(x){if(flags & x) result += "|" #x;}
  ShowSymGetOptions_CONVERT(SYMOPT_ALLOW_ABSOLUTE_SYMBOLS);
  ShowSymGetOptions_CONVERT(SYMOPT_AUTO_PUBLICS);
  ShowSymGetOptions_CONVERT(SYMOPT_CASE_INSENSITIVE);
  ShowSymGetOptions_CONVERT(SYMOPT_DEBUG);
  ShowSymGetOptions_CONVERT(SYMOPT_DEFERRED_LOADS);
  ShowSymGetOptions_CONVERT(SYMOPT_EXACT_SYMBOLS);
  ShowSymGetOptions_CONVERT(SYMOPT_FAIL_CRITICAL_ERRORS);
  ShowSymGetOptions_CONVERT(SYMOPT_IGNORE_CVREC);
  ShowSymGetOptions_CONVERT(SYMOPT_IGNORE_NT_SYMPATH);
  ShowSymGetOptions_CONVERT(SYMOPT_INCLUDE_32BIT_MODULES);
  ShowSymGetOptions_CONVERT(SYMOPT_LOAD_ANYTHING);
  ShowSymGetOptions_CONVERT(SYMOPT_LOAD_LINES);
  ShowSymGetOptions_CONVERT(SYMOPT_NO_CPP);
  ShowSymGetOptions_CONVERT(SYMOPT_NO_IMAGE_SEARCH);
  ShowSymGetOptions_CONVERT(SYMOPT_NO_PUBLICS);
  ShowSymGetOptions_CONVERT(SYMOPT_NO_UNQUALIFIED_LOADS);
  ShowSymGetOptions_CONVERT(SYMOPT_PUBLICS_ONLY);
  ShowSymGetOptions_CONVERT(SYMOPT_SECURE);
  ShowSymGetOptions_CONVERT(SYMOPT_UNDNAME);
#undef ShowSymGetOptions_CONVERT
  HW_D_LOG_VALUE(result);
}


#define GetFrame(data) !!::StackWalk64(IMAGE_FILE_MACHINE_I386,Process,thread.Handle,data,0,0,SymFunctionTableAccess64,SymGetModuleBase64,0)

StackTrace::StackTrace(const Thread& thread)
{
  static bool symInitializeCalled = false;
  if(!symInitializeCalled)
  {
    symInitializeCalled = true;
    WinApiCall(::SymInitialize(Process, 0, TRUE));
    ShowSymGetOptions();
  }

  MemBox<CONTEXT> context;
  MemBox<STACKFRAME64> data;
  context->ContextFlags = CONTEXT_CONTROL | CONTEXT_INTEGER | CONTEXT_SEGMENTS | CONTEXT_DEBUG_REGISTERS;
  WinApiCall(::GetThreadContext(thread.Handle, context));

  data->AddrPC.Offset = context->Rip;
  data->AddrPC.Mode = AddrModeFlat;
  data->AddrStack.Offset = context->Rsp;
  data->AddrStack.Mode = AddrModeFlat;
  data->AddrFrame.Offset = context->Rbp;
  data->AddrFrame.Mode = AddrModeFlat;

  auto isValid = GetFrame(data);
  // First layer has to be dropped
  while(isValid && data->AddrFrame.Offset != 0)
  {
    isValid = GetFrame(data);
    StackFrameData sf(data->AddrPC.Offset);
    frames += sf;
  }
};


string StackTrace::Format(size_t StartLevel) const
{
  string Return;;
  for(size_t i = StartLevel, n = frames.Count - 1; i < n; i++)
  {
    Return += frames[i].Format();
    Return += "\n";
  };
  return __FUNCTION__ + ("\n" + Return)|Indent();
};


class Thread::StackTraceThread
{
  const int level;
  string value;
  Thread thread;
public:
  StackTraceThread(int level)
    : level(level + 1)
      , thread(get_Current())
  {
    Thread traceThread = CreateThread(
      nullptr,
      0,
      ThreadProc,
      this,
      DETACHED_PROCESS,
      nullptr
    );

    while(value == "")
      Sleep(50);
  };

  p(string, Value)
  {
    return value;
  };

private:
  static string _tempValue;

  static DWORD WINAPI ThreadProc(LPVOID RawThis)
  {
    reinterpret_cast<StackTraceThread*>(RawThis)->Execute();
    return 0;
  };

  void Execute()
  {
    thread.Suspend();
    value = StackTrace(thread).Format(level + 1);
    thread.Resume();
  };
};


Thread Thread::get_Current()
{
  HANDLE result;
  WinApiCall
  (
    ::DuplicateHandle
    (
      ::GetCurrentProcess(),
      ::GetCurrentThread(),
      ::GetCurrentProcess(),
      &result,
      0,
      false,
      DUPLICATE_SAME_ACCESS
    )
  );
  return result;
}

string Thread::FormatStackTraceOfCurrentThread(int level) { return StackTraceThread(level + 1).Value; }

void Thread::Suspend() const
{
  WinApiCall(::SuspendThread(handle) + 1);
};

void Thread::Resume() const
{
  WinApiCall(::ResumeThread(handle));
};
