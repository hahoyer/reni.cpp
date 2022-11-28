#include "Import.h"
#include "Process.h"

#include "DumpableObject.h"
#include <cstring>
#include "System.h"
#include <windows.h>

static bool Trace = true;
using namespace HWLib;
using namespace std;


class Process::internal
{
public:
  class Pipe
  {
    static constexpr int BufferSize = 4096;

  public:
    mutable HANDLE ReadHandle;
    mutable HANDLE WriteHandle;

    Pipe();
    ~Pipe();
    HW_PR_VAR(string, Data);

  private:
    static void CloseHandle(HANDLE& handle);
  };

  const string Command{};
  Pipe _fromChild;
  Pipe _errorFromChild;
  bool isValid;
  string ApplicationName{};
  void Ensure();
  void Reset();

  static string GetCommandProcessorName()
  {
    char result[2000];
    ::GetEnvironmentVariable("ComSpec", result, 2000);
    return result;
  };

public:
  internal(const string& applicationName, const string& command)
    : Command(command)
      , isValid(false)
      , ApplicationName(applicationName) {}

  internal(const string& command)
    : Command("/c " + command)
      , isValid(false)
      , ApplicationName(GetCommandProcessorName()) {}

  string data{};
  string errorData{};
  int Result = 0;
  HW_PR_VAR(bool, IsValid) { return isValid; }
};

HW_PR_IMPL_SETTER(Process::internal, bool, IsValid)
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

HW_PR_IMPL_GETTER(Process::internal::Pipe, string, Data)
{
  CloseHandle(WriteHandle);
  string result;
  for(;;)
  {
    CHAR buffer[BufferSize];
    DWORD bytesRead = 0;
    const BOOL Success = ReadFile(ReadHandle, buffer, BufferSize, &bytesRead, nullptr);
    if(bytesRead)
      result += string(buffer,bytesRead);
    if(!Success || bytesRead == 0)
      return result;
  }
}

HW_PR_IMPL_SETTER(Process::internal::Pipe, string, Data)
{
  DWORD bytesWritten;
  WriteFile(WriteHandle, value.c_str(), static_cast<DWORD>(value.size()), &bytesWritten, nullptr);
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

  string commandChars = Command.data();
  if(::CreateProcess(
    ApplicationName.c_str(),
    string(Command.c_str()).data(),
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

Process::Process(const string& command) : _internal(new internal(command)) {};
Process::Process(const string& applicationName, const string& command) : _internal(new internal(applicationName, command)) {};

HW_PR_IMPL_GETTER(Process, string, data)
{
  const_cast<Process&>(*this)._internal->IsValid = true;
  return _internal->data;
};

HW_PR_IMPL_GETTER(Process, string, errorData)
{
  const_cast<Process&>(*this)._internal->IsValid = true;
  return _internal->errorData;
};

HW_PR_IMPL_GETTER(Process, int, result)
{
  const_cast<Process&>(*this)._internal->IsValid = true;
  return _internal->Result;
};

void Process::Execute()
{
  _internal->IsValid = false;
  _internal->IsValid = true;
}
