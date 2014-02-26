#pragma once
#include "..\HWLib\System.h"
#include "..\HWLib\File.h"

using namespace HWLib;

namespace Reni
{
    class CppCompilerScripting 
    {
        using thisType = CppCompilerScripting;
        String const cppCode;
    public:
        CppCompilerScripting(String const& cppCode);
        void Execute();
        p(String, program);
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

