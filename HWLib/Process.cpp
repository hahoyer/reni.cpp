#include "Import.h"
#include "Process.h"

#include "DumpableObject.h"
#include "String.h"
#include "System.h"
#include <windows.h>

static bool Trace = true;
using namespace HWLib;


class Process::internal
{
public:
  class Pipe
  {
    static const int BUFSIZE = 4096;

  public:
    mutable HANDLE ReadHandle;
    mutable HANDLE WriteHandle;

    Pipe();
    ~Pipe();
    p_mutable(String, Data);

  private:
    static void CloseHandle(HANDLE& handle);
  };

  const String Command;
  Pipe _fromChild;
  Pipe _errorFromChild;
  bool isValid;
  String ApplicationName;
  void Ensure();
  void Reset();

  static String GetCommandProcessorName()
  {
    char result[2000];
    ::GetEnvironmentVariable("ComSpec", result, 2000);
    return result;
  };

public:
  internal(const String& applicationName, const String& command)
    : ApplicationName(applicationName)
      , Command(command)
      , isValid(false) {}

  internal(const String& command)
    : ApplicationName(GetCommandProcessorName())
      , Command("/c " + command)
      , isValid(false) {}

  String data;
  String errorData;
  int Result = 0;
  p_mutable(bool, IsValid) { return isValid; }
};

p_mutator_implementation(Process::internal, bool, IsValid)
{
  if(value)
    Ensure();
  else
    Reset();
};


Process::internal::Pipe::Pipe()
{
  ReadHandle = nullptr;
  WriteHandle = nullptr;
  SECURITY_ATTRIBUTES saAttr;
  saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
  saAttr.bInheritHandle = TRUE;
  saAttr.lpSecurityDescriptor = nullptr;
  CreatePipe(&ReadHandle, &WriteHandle, &saAttr, 0);
}

Process::internal::Pipe::~Pipe()
{
  CloseHandle(WriteHandle);
  CloseHandle(ReadHandle);
}

p_implementation(Process::internal::Pipe, String, Data)
{
  CloseHandle(WriteHandle);
  String result;
  for(;;)
  {
    CHAR buffer[BUFSIZE];
    DWORD bytesRead = 0;
    const BOOL Success = ReadFile(ReadHandle, buffer, BUFSIZE, &bytesRead, nullptr);
    if(bytesRead)
      result += String(bytesRead, buffer);
    if(!Success || bytesRead == 0)
      return result;
  }
}

p_mutator_implementation(Process::internal::Pipe, String, Data)
{
  DWORD bytesWritten;
  WriteFile(WriteHandle, value.RawData, static_cast<DWORD>(value.Count), &bytesWritten, nullptr);
  CloseHandle(WriteHandle);
  CloseHandle(ReadHandle);
}

void Process::internal::Pipe::CloseHandle(HANDLE& handle)
{
  if(handle == nullptr)
    return;
  ::CloseHandle(handle);
  handle = nullptr;
}

void Process::internal::Ensure()
{
  if(isValid)
    return;
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  ZeroMemory(&si, sizeof si);
  si.cb = sizeof si;
  si.hStdOutput = _fromChild.WriteHandle;
  si.hStdError = _errorFromChild.WriteHandle;
  si.dwFlags |= STARTF_USESTDHANDLES;
  ZeroMemory(&pi, sizeof pi);

  const auto commandChars = Command.RawDataCopy;
  if(::CreateProcess(
    ApplicationName.RawData,
    commandChars,
    nullptr, // 		__in_opt     LPSECURITY_ATTRIBUTES lpProcessAttributes,
    nullptr, // 		__in_opt     LPSECURITY_ATTRIBUTES lpThreadAttributes,
    TRUE, // 		__in         BOOL bInheritHandles,
    0, // 		__in         DWORD dwCreationFlags,
    nullptr, // 		__in_opt     LPVOID lpEnvironment,
    ".", // 		__in_opt     LPCTSTR <	lpCurrentDirectory,
    &si, // 		__in         LPSTARTUPINFO lpStartupInfo,
    &pi // 		__out        LPPROCESS_INFORMATION lpProcessInformation
  ) == 0)
    System::ThrowLastErrorMessage();;

  delete[] commandChars;

  data = _fromChild.Data;
  errorData = _errorFromChild.Data;
  DWORD result;
  GetExitCodeProcess(pi.hProcess, &result);
  this->Result = result;
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
  isValid = true;
};


void Process::internal::Reset()
{
  if(isValid)
  {
    data = "";
    errorData = "";
    isValid = false;
  };
};

Process::Process(const String& command) : _internal(new internal(command)) {};
Process::Process(const String& applicationName, const String& command) : _internal(new internal(applicationName, command)) {};

p_implementation(Process, String, data)
{
  const_cast<Process&>(*this)._internal->IsValid = true;
  return _internal->data;
};

p_implementation(Process, String, errorData)
{
  const_cast<Process&>(*this)._internal->IsValid = true;
  return _internal->errorData;
};

p_implementation(Process, int, result)
{
  const_cast<Process&>(*this)._internal->IsValid = true;
  return _internal->Result;
};

void Process::Execute()
{
  _internal->IsValid = false;
  _internal->IsValid = true;
}
