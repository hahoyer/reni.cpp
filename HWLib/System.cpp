#include "Import.h"
#include "System.h"

#include "String.h"
#include "TemplateInstances.h"

static bool Trace = true;
using namespace HWLib;


String const System::FormatLastErrorMessage()
{
    auto rc = ::GetLastError();
    char Buffer[3000];
    ::FormatMessage
        (
            FORMAT_MESSAGE_FROM_SYSTEM,
                                      NULL,
                                      rc,
                                      0,
                                      Buffer, 3000,
                                      NULL
        );
    return String::Convert(int(rc)) + ": " + Buffer;
};

void System::ThrowLastErrorMessage()
{
    auto message = FormatLastErrorMessage();
    throw runtime_error(message.RawData);
};

String const System::EnvironmentVariable(String const&key)
{
    static const int bufferSize = 65535;
    char module[bufferSize];
    auto val = ::GetEnvironmentVariable(key.RawData, module, bufferSize);
    return module;
}

void System::Sleep(int milliseconds)
{
    ::Sleep(milliseconds);
}

String const System::ModuleName(unsigned __int64 instructionPointer)
{
    PVOID instructionAddress = reinterpret_cast<LPVOID>(instructionPointer);
    MEMORY_BASIC_INFORMATION information;
    int rc = ::VirtualQuery(instructionAddress, &information, sizeof(information));
    assert(rc);
    HMODULE hMod = reinterpret_cast<HMODULE>(information.AllocationBase);
    char ModuleName[MAX_PATH];
    if(GetModuleFileName(hMod, ModuleName, sizeof(ModuleName)))
        return ModuleName;
    return "";
};


__int64 const System::Ticks()
{
    LARGE_INTEGER x;
    BOOL rc = ::QueryPerformanceCounter(&x);
    return x.QuadPart;
};

__int64 const System::TicksPerSecond()
{
    LARGE_INTEGER x;
    BOOL rc = ::QueryPerformanceFrequency(&x);
    return x.QuadPart;
};