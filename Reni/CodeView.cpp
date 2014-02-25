#include "Import.h"
#include "CodeView.h"

using namespace Reni;
static bool Trace = true;


CodeView::CodeView(String const& cppCode) : cppCode(cppCode){}

p_implementation(CodeView, String, program)
{
    static String const result = R"(
    #include "Admin/Export.h"
    using namespace ReniRuntime; 
    int main(void){
        {0}
        return 0;
    }
    )";
    return result.Replace("{0}", cppCode);
};

p_implementation(CodeView, String, fullFileName)
{
    auto temp = System::EnvironmentVariable("TEMP");
    File tempDir = temp + "\\reni";
    tempDir.IsValidFolder = true;
    return tempDir.FullName + "\\" + fileName;
};

void CodeView::InitializeFile()
{
    File f = fullFileName + ".cpp";
    f.Data = cppCode;
};

void CodeView::Execute()
{
    Process("vcvars32").Execute();
    InitializeFile();
    _console_ Write("Compiling:\n");
    auto cpp = fullFileName + ".cpp";
    auto exe = fullFileName + ".exe";
    d(cpp);
    d(exe);

    _console_ Write(CompileCommand + "\n");
    Process pcpp(CompileCommand);
    auto compileResult = pcpp.data;
    auto error = pcpp.errorData;
    if ((compileResult == "" || compileResult == fileName + ".cpp\r\n") && error == ""){
        Process pexe(exe);
        auto result = pexe.data;
        d(result);
        d(pexe.errorData);
        b_;
    };

    dd("compileResult: " + compileResult);
    dd("error: " + error);
    b_;
};


p_implementation(CodeView, String, CompileCommand){
    auto result
        = String()
        + R"(cl.exe )"
        + R"(/Zi /Zl /nologo /W4 /WX- /sdl /MP8 /Od /Oi /Oy- /D _DEBUG /Gm- /EHsc /RTC1 )"
        + R"(/MDd /GS /Gy- /fp:precise /Zc:wchar_t /Zc:forScope /GR /Gd /TP /analyze- /FC /errorReport:prompt )"
        + fullFileName + ".cpp "
        + Includes + " "
        + R"(/link /NOLOGO /NODEFAULTLIB /DEBUG /SUBSYSTEM:CONSOLE /OPT:NOREF /MACHINE:X86 )"
        + "/OUT:\"" + fullFileName + ".exe\" "
        //+ R"(kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib )"
        ;
    return result;
};

p_implementation(CodeView, String, Includes){
    Array<String>includeDirs {
        get_VCInstallDir() + "include",
        get_Boost(), 
        get_RuntimeDir()
    };

    return includeDirs
        .Select<String>([](String dir){return "/I\"" + dir + "\""; })
        ->Stringify(" ");
};
