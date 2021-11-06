#include "Import.h"
#include "CppCompilerScripting.h"
#include "../HWLib/Array.h"

using namespace Util;
using namespace HWLib;
static bool Trace = true;

CppCompilerScripting::CppCompilerScripting(const string& program)
  : program(program)
    , currentProcess("echo none")
{}


p_implementation(CppCompilerScripting, string, fullFileName)
{
  const auto temp = System::EnvironmentVariable("TEMP");
  File tempDir = temp + "\\reni";
  tempDir.IsValidFolder = true;
  return tempDir.FullName + "\\" + fileName;
};

void CppCompilerScripting::InitializeFile() const
{
  File f = fullFileName + ".cpp";
  f.Data = program;
};

void CppCompilerScripting::Execute()
{
  const bool Trace = true;
  InitializeFile();
  const auto cpp = fullFileName + ".cpp";
  const auto exe = fullFileName + ".exe";
  HW_D_LOG_VALUE(cpp);
  HW_D_LOG_VALUE(exe);

  HW_D_LOG_VALUE(CppCompilerExecutable);
  HW_D_LOG(CompileCommand + "\n");
  currentProcess = Process(CppCompilerExecutable, CompileCommand);
  if(currentProcess.result == 0)
  {
    currentProcess = Process(exe,"");
    currentProcess.Execute();
  }

  HW_D_LOG_VALUE(currentProcess.result);
  HW_D_LOG_VALUE(currentProcess.data);
  HW_D_LOG_VALUE(currentProcess.errorData);
  if(currentProcess.errorData != "")
  {
    auto error = currentProcess.errorData;
    throw *this;
  }
};


p_implementation(CppCompilerScripting, int, result)
{
  return currentProcess.result;
};

p_implementation(CppCompilerScripting, string, output)
{
  return currentProcess.data;
};

p_implementation(CppCompilerScripting, string, LibPath)
{
  const Array<string> list{
    VCInstallDir + R"(\lib\)" + Platform,
    WindowsKits + R"(\lib\)" + CrtVersion + R"(\um\)" + Platform,
    WindowsKits + R"(\lib\)" + CrtVersion + R"(\ucrt\)" + Platform,
  };

  return list
         .Select<string>([&](string name) { return "/LIBPATH:\"" + name + "\""; })
         ->Stringify(" ");
};

p_implementation(CppCompilerScripting, string, CompileCommand)
{
  return
    R"(/Zi /Zl /nologo /W4 /WX- /sdl /MP8 /Od /Oi /Oy- /D _DEBUG /Gm- /EHsc /MDd /GS /Gy- /fp:precise /Zc:wchar_t /Zc:forScope /GR /TP /analyze- /FC )"
    + fullFileName + ".cpp "
    + IncludePath + " "
    + R"(/link /NOLOGO /DEBUG /OPT:NOREF /MACHINE:)" + Platform + " "
    + LibPath + " "
    + "MSVCRTD.lib "
    + "/OUT:\"" + fullFileName + ".exe\" ";
};

p_implementation(CppCompilerScripting, string, IncludePath)
{
  const Array<string> list{
    VCInstallDir + "\\include",
    WindowsKits + R"(\include\)" + CrtVersion + R"(\ucrt)",
    RuntimeDir
  };

  return list
         .Select<string>([](string dir) { return "/I\"" + dir + "\""; })
         ->Stringify(" ");
};
