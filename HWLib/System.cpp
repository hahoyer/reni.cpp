#include "Import.h"
#include "System.h"

#include <cassert>


#include "String.h"
#include "TemplateInstances.h"
#include <stdexcept>

static bool Trace = true;
using namespace HWLib;


std::string System::FormatLastErrorMessage()
{
  const auto rc = GetLastError();
  char Buffer[3000];
  ::FormatMessage
  (
    FORMAT_MESSAGE_FROM_SYSTEM,
    nullptr,
    rc,
    0,
    Buffer, 3000,
    nullptr
  );
  return String::Convert(static_cast<int>(rc)) + ": " + Buffer;
};

void System::ThrowLastErrorMessage()
{
  const auto message = FormatLastErrorMessage();
  throw runtime_error(message.c_str());
};

std::string System::EnvironmentVariable(const String& key)
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

std::string System::ModuleName(unsigned __int64 instructionPointer)
{
  const auto instructionAddress = reinterpret_cast<LPVOID>(instructionPointer);
  MEMORY_BASIC_INFORMATION information;
  const auto rc = VirtualQuery(instructionAddress, &information, sizeof information);
  assert(rc);
  auto* const hMod = static_cast<HMODULE>(information.AllocationBase);
  char ModuleName[MAX_PATH];
  if(GetModuleFileName(hMod, ModuleName, sizeof ModuleName))
    return ModuleName;
  return "";
};


__int64 System::Ticks()
{
  LARGE_INTEGER x;
  BOOL rc = QueryPerformanceCounter(&x);
  return x.QuadPart;
};

__int64 System::TicksPerSecond()
{
  LARGE_INTEGER x;
  BOOL rc = QueryPerformanceFrequency(&x);
  return x.QuadPart;
};
