#include "Import.h"
#include "System.h"

#include "String.h"
#include "TemplateInstances.h"
#include <windows.h>

static bool Trace = true;
using namespace HWLib;


String const System::FormatLastErrorMessage()
{
    DWORD rc = ::GetLastError();
    char Buffer[3000];
    ::FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        rc,
        0,
        Buffer, 3000,
        NULL);
    return String::Convert(int(rc)) + ": " + Buffer;
};


String const System::EnvironmentVariable(String const&key){
    static const int bufferSize = 65535;
    char module[bufferSize];
    auto val = ::GetEnvironmentVariable(key.RawData, module, bufferSize);
    return module;
}


void System::Sleep(int milliseconds){
    ::Sleep(milliseconds);
}

