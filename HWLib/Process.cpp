#include "Import.h"
#include "Process.h"

#include "DumpableObject.h"
#include "String.h"
#include "System.h"
#include <windows.h>

static bool Trace = true;
using namespace HWLib;


class Process::internal{
public:
    class Pipe{
        static const int BUFSIZE = 4096;

    public:
        mutable HANDLE ReadHandle;
        mutable HANDLE WriteHandle;

        Pipe();
        ~Pipe();
        p_mutable(String, Data);

    private:
        void CloseHandle(HANDLE& handle)const;
    };

    String const _command;
    Pipe _fromChild;
    Pipe _errorFromChild;
    bool isValid;
    void Ensure();
    void Reset();
public:
    internal(String const& command)
        : _command(command)
        , isValid(false){}

    String data;
    String errorData;
    int result;
    p_mutable(bool, IsValid){ return isValid; }
};

p_mutator_implementation(Process::internal, bool, IsValid)
{
    if (value)
        Ensure();
    else
        Reset();
};


Process::internal::Pipe::Pipe()
{
    ReadHandle = 0;
    WriteHandle = 0;
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = 0;
    ::CreatePipe(&ReadHandle, &WriteHandle, &saAttr, 0);
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
    for (;;)
    {
        CHAR buffer[BUFSIZE];
        DWORD bytesRead = 0;
        BOOL Success = ::ReadFile(ReadHandle, buffer, BUFSIZE, &bytesRead, NULL);
        if (bytesRead)
            result += String(bytesRead, buffer);
        if (!Success || bytesRead == 0)
            return result;
    }
}

p_mutator_implementation(Process::internal::Pipe, String, Data)
{
    DWORD bytesWritten;
    ::WriteFile(WriteHandle, value.RawData, value.Count, &bytesWritten, NULL);
    CloseHandle(WriteHandle);
    CloseHandle(ReadHandle);
}

void Process::internal::Pipe::CloseHandle(HANDLE& handle)const
{
    if (handle == 0)
        return;
    ::CloseHandle(handle);
    handle = 0;
}


void Process::internal::Ensure()
{
    if (isValid)
        return;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdOutput = _fromChild.WriteHandle;
    si.hStdError = _errorFromChild.WriteHandle;
    si.dwFlags |= STARTF_USESTDHANDLES;
    ZeroMemory(&pi, sizeof(pi));

    char module[2000];
    auto val = ::GetEnvironmentVariable("ComSpec", module, 2000);
    auto commandChars = ("/c_ " + _command).RawDataCopy;
    if (::CreateProcess(
        module,
        commandChars,
        0, // 		__in_opt     LPSECURITY_ATTRIBUTES lpProcessAttributes,
        0, // 		__in_opt     LPSECURITY_ATTRIBUTES lpThreadAttributes,
        TRUE,// 		__in         BOOL bInheritHandles,
        0,// 		__in         DWORD dwCreationFlags,
        0,// 		__in_opt     LPVOID lpEnvironment,
        0,// 		__in_opt     LPCTSTR <	lpCurrentDirectory,
        &si,// 		__in         LPSTARTUPINFO lpStartupInfo,
        &pi// 		__out        LPPROCESS_INFORMATION lpProcessInformation
        ) == 0)
    {
        System::ThrowLastErrorMessage();
    };

    delete[] commandChars;

    data = _fromChild.Data;
    errorData = _errorFromChild.Data;
    DWORD result;
    ::GetExitCodeProcess(pi.hProcess, &result);
    this->result = result;
    ::CloseHandle(pi.hProcess);
    ::CloseHandle(pi.hThread);
    isValid = true;
};


void Process::internal::Reset()
{
    if (isValid){
        data = "";
        errorData = "";
        isValid = false;
    };
};

Process::Process(String const&command) : _internal(new internal(command)){};

p_implementation(Process, String, data){
    const_cast<Process&>(*this)._internal->IsValid = true;
    return _internal->data;
};

p_implementation(Process, String, errorData){
    const_cast<Process&>(*this)._internal->IsValid = true;
    return _internal->errorData;
};

p_implementation(Process, int, result){
    const_cast<Process&>(*this)._internal->IsValid = true;
    return _internal->result;
};

void Process::Execute(){
    _internal->IsValid = false;
    _internal->IsValid = true;
}

