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
    HW_PR_GET(int, result);
    HW_PR_GET(string, output);
  private:
    HW_PR_GET(string, fullFileName);
    HW_PR_GET(string, fileName) { return "temp"; };
    HW_PR_GET(string, ProgramFiles86) { return System::EnvironmentVariable("ProgramFiles(x86)"); };
    HW_PR_GET(string, VCInstallDir) { return System::EnvironmentVariable("VS142BASEPATH"); };
    HW_PR_GET(string, WindowsKits) { return ProgramFiles86 + R"(\Windows Kits\10)"; };
    HW_PR_GET(string, ProjectDir) { return File(__FILE__ "\\..").FullName; };
    HW_PR_GET(string, SolutionDir) { return File(ProjectDir + "\\..").FullName; };
    HW_PR_GET(string, RuntimeDir) { return File(SolutionDir + "\\Runtime").FullName; };
    HW_PR_GET(string, IncludePath);
    HW_PR_GET(string, LibPath);
    HW_PR_GET(string, CppCompilerExecutable) { return VCInstallDir + R"(\bin\HostX64\x64\cl.exe)"; };
    HW_PR_GET(string, CompileCommand);
    HW_PR_GET(string, LinkerCommand);
    HW_PR_GET(string, Platform) { return "x64"; };
    HW_PR_GET(string, CrtVersion) { return "10.0.20348.0"; };

    void InitializeFile() const;
  };
}
