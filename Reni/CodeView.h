#pragma once

namespace Reni
{
    class CodeView 
    {
        using thisType = CodeView;
        String const cppCode;
    public:
        CodeView(String const& cppCode);
        void Execute();
        p(String, program);
    private:
        p(String, fullFileName);
        p(String, fileName){return "temp";};
        p(String, ProgramFiles86){ return System::EnvironmentVariable("ProgramFiles(x86)"); };
        p(String, Boost){ return ProgramFiles86 + "\\boost_1_55_0"; };
        p(String, VCInstallDir){ return R"(C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\)"; };
        p(String, ProjectDir){ return File(__FILE__ "\\..").FullName; };
        p(String, SolutionDir){ return File(ProjectDir + "\\..").FullName; };
        p(String, RuntimeDir){ return File(SolutionDir + "\\Runtime").FullName; };
        p(String, Includes);
        p(String, Libs);
        p(String, CompileCommand);
        p(String, LinkerCommand);

        void InitializeFile();
    };

}

