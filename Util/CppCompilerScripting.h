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
    const String program;
    Process currentProcess;
  public:
    CppCompilerScripting(const String& program);

    void Execute();
    p(int, result);
    p(String, output);
  private:
    p(String, fullFileName);
    p(String, fileName) { return "temp"; };
    p(String, ProgramFiles86) { return System::EnvironmentVariable("ProgramFiles(x86)"); };
    p(String, VCInstallDir) { return System::EnvironmentVariable("VS142BASEPATH"); };
    p(String, WindowsKits) { return ProgramFiles86 + R"(\Windows Kits\10)"; };
    p(String, Boost) { return ProgramFiles86 + "\\boost\\boost_1_77_0"; };
    p(String, ProjectDir) { return File(__FILE__ "\\..").FullName; };
    p(String, SolutionDir) { return File(ProjectDir + "\\..").FullName; };
    p(String, RuntimeDir) { return File(SolutionDir + "\\Runtime").FullName; };
    p(String, IncludePath);
    p(String, LibPath);
    p(String, CppCompilerExecutable) { return VCInstallDir + "\\bin\\HostX64\\x64\\cl.exe"; };
    p(String, CompileCommand);
    p(String, LinkerCommand);
    p(String, Platform){return "x64";};
    p(String, CrtVersion){return "10.0.20348.0";};

    void InitializeFile() const;
  };
}
