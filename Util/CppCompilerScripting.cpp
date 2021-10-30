#include "Import.h"
#include "CppCompilerScripting.h"
#include "../HWLib/Array.h"

using namespace Util;
using namespace HWLib;
static bool Trace = true;

CppCompilerScripting::CppCompilerScripting(const String& program)
  : program(program)
    , currentProcess("echo none")
{}


p_implementation(CppCompilerScripting, String, fullFileName)
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
  d(cpp);
  d(exe);

  d(CppCompilerExecutable);
  dd(CompileCommand + "\n");
  currentProcess = Process(CppCompilerExecutable, CompileCommand);
  if(currentProcess.result == 0)
  {
    currentProcess = Process(exe,"");
    currentProcess.Execute();
  }

  d(currentProcess.result);
  d(currentProcess.data);
  d(currentProcess.errorData);
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

p_implementation(CppCompilerScripting, String, output)
{
  return currentProcess.data;
};

p_implementation(CppCompilerScripting, String, LibPath)
{
  const Array<String> list{
    VCInstallDir + R"(\lib\)" + Platform,
    WindowsKits + R"(\lib\)" + CrtVersion + R"(\um\)" + Platform,
    WindowsKits + R"(\lib\)" + CrtVersion + R"(\ucrt\)" + Platform,
    Boost + "\\libs",
  };

  return list
         .Select<String>([&](String name) { return "/LIBPATH:\"" + name + "\""; })
         ->Stringify(" ");
};

p_implementation(CppCompilerScripting, String, CompileCommand)
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

p_implementation(CppCompilerScripting, String, IncludePath)
{
  const Array<String> list{
    VCInstallDir + "\\include",
    WindowsKits + R"(\include\)" + CrtVersion + R"(\ucrt)",
    Boost,
    RuntimeDir
  };

  return list
         .Select<String>([](String dir) { return "/I\"" + dir + "\""; })
         ->Stringify(" ");
};
