#include "Import.h"
#include "CppCompilerScripting.h"

using namespace Reni;
static bool Trace = true;

CppCompilerScripting::CppCompilerScripting(String const& cppCode) : cppCode(cppCode){}

p_implementation(CppCompilerScripting, String, program)
{
    static String const result = R"(
    #include "Common.h"
    using namespace ReniRuntime; 
    int main(void){
        {0}
        return 0;
    }
    )";
    return result.Replace("{0}", cppCode);
};

p_implementation(CppCompilerScripting, String, fullFileName)
{
    auto temp = System::EnvironmentVariable("TEMP");
    File tempDir = temp + "\\reni";
    tempDir.IsValidFolder = true;
    return tempDir.FullName + "\\" + fileName;
};

void CppCompilerScripting::InitializeFile()
{
    File f = fullFileName + ".cpp";
    f.Data = cppCode;
};

void CppCompilerScripting::Execute()
{
    InitializeFile();
    _console_ Write("Compiling:\n");
    auto cpp = fullFileName + ".cpp";
    auto exe = fullFileName + ".exe";
    d(cpp);
    d(exe);

    _console_ Write(CompileCommand + "\n");
    Process pcpp(CompileCommand);
    if (pcpp.result)
    {
        auto compileResult = pcpp.data;
        auto error = pcpp.errorData;
        dd("compileResult: " + compileResult);
        dd("error: " + error);
        b_;

        return;
    }

    Process pexe(exe);
    pexe.Execute();
    d(pexe.result);
    d(pexe.data);
    d(pexe.errorData);
    b_;

};

p_implementation(CppCompilerScripting, String, LibPath){
    Array<String>list{
        VCInstallDir + R"(\lib)",
        ProgramFiles86 + R"(\Windows Kits\8.1\lib\winv6.3\um\x86)",
        Boost + "\\libs",
    };

    return list
        .Select<String>([&](String name){return "/LIBPATH:\"" + name + "\""; })
        ->Stringify(" ");
};

p_implementation(CppCompilerScripting, String, CompileCommand){
    return
        String()
        + R"(cl.exe )"
        + R"(/Zi /Zl /nologo /W4 /WX- /sdl /MP8 /Od /Oi /Oy- /D _DEBUG /Gm- /EHsc /MDd /GS /Gy- /fp:precise /Zc:wchar_t /Zc:forScope /GR /TP /analyze- /FC )"
        + fullFileName + ".cpp "
        + IncludePath + " "
        + R"(/link /NOLOGO /DEBUG /OPT:NOREF /MACHINE:X86 )"
        + LibPath + " "
        + R"(MSVCRTD.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib )"
        + "/OUT:\"" + fullFileName + ".exe\" "
        ;
};

p_implementation(CppCompilerScripting, String, IncludePath){
    Array<String>list{
        get_VCInstallDir() + "\\include",
        get_Boost(),
        get_RuntimeDir()
    };

    return list
        .Select<String>([](String dir){return "/I\"" + dir + "\""; })
        ->Stringify(" ");
};
