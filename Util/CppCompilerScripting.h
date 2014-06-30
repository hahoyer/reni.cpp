#pragma once
#include "..\HWLib\System.h"
#include "..\HWLib\File.h"
#include "..\HWLib\Process.h"

using namespace HWLib;

namespace Util{
    class CppCompilerScripting
    {
        using thisType = CppCompilerScripting;
        String const program;
        Process currentProcess;
    public:
        CppCompilerScripting(String const& program);

        void Execute();
        p(int, result);
        p(String, output);
    private:
        p(String, fullFileName);
        p(String, fileName){return "temp";};
        p(String, ProgramFiles86){ return HWLib::System::EnvironmentVariable("ProgramFiles(x86)"); };
        p(String, Boost){ return ProgramFiles86 + "\\boost_1_55_0"; };
        p(String, VCInstallDir){ return ProgramFiles86 + "\\Microsoft Visual Studio 12.0\\VC"; };
        p(String, ProjectDir){ return File(__FILE__ "\\..").FullName; };
        p(String, SolutionDir){ return File(ProjectDir + "\\..").FullName; };
        p(String, RuntimeDir){ return File(SolutionDir + "\\Runtime").FullName; };
        p(String, IncludePath);
        p(String, LibPath);
        p(String, CompileCommand);
        p(String, LinkerCommand);

        void InitializeFile();
    };

}

