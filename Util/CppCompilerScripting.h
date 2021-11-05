#pragma once
#include "..\HWLib\System.h"
#include "..\HWLib\File.h"
#include "..\HWLib\Process.h"

using namespace HWLib;

namespace Util
{
  class CppCompilerScripting
  {
    using thisType = CppCompilerScripting;
    const string program;
    Process currentProcess;
  public:
    CppCompilerScripting(const string& program);

    void Execute();
    p(int, result);
    p(string, output);
  private:
    p(string, fullFileName);
    p(string, fileName) { return "temp"; };
    p(string, ProgramFiles86) { return System::EnvironmentVariable("ProgramFiles(x86)"); };
    p(string, VCInstallDir) { return System::EnvironmentVariable("VS142BASEPATH"); };
    p(string, WindowsKits) { return ProgramFiles86 + R"(\Windows Kits\10)"; };
    p(string, ProjectDir) { return File(__FILE__ "\\..").FullName; };
    p(string, SolutionDir) { return File(ProjectDir + "\\..").FullName; };
    p(string, RuntimeDir) { return File(SolutionDir + "\\Runtime").FullName; };
    p(string, IncludePath);
    p(string, LibPath);
    p(string, CppCompilerExecutable) { return VCInstallDir + R"(\bin\HostX64\x64\cl.exe)"; };
    p(string, CompileCommand);
    p(string, LinkerCommand);
    p(string, Platform) { return "x64"; };
    p(string, CrtVersion) { return "10.0.20348.0"; };

    void InitializeFile() const;
  };
}
