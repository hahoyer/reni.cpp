#include "Import.h"
#include "CodeView.h"

using namespace Reni;
static bool Trace = true;


CodeView::CodeView(String const& cppCode) : cppCode(cppCode){}

p_implementation(CodeView, String, program)
{
    static String const result = R"(
    #include <stdlib.h>
    int main(void){
        {0}
        return 0;
    }
    )";
    return result.Replace("{0}", cppCode);
};

p_implementation(CodeView, String, fileName)
{
    auto temp = System::EnvironmentVariable("TEMP");
    File tempDir = temp + "\\reni";
    tempDir.IsValidFolder = true;
    return tempDir.FullName + "\\temp.cpp";
};

void CodeView::InitializeFile()
{
    File f = fileName;
    f.Data = cppCode;
};

void CodeView::Execute()
{
    InitializeFile();
    Process ccc("cl " + fileName);
    auto compileResult = ccc.data;
    auto error = ccc.errorData;
    if ((compileResult == "" || compileResult == fileName + "\r\n") && error == ""){
        auto result = Process(fileName + ".exe").data;
        d(result);
        b_;
    };

    dd(compileResult);
    dd(error);
    b_;
};

void Execute()
{
    String const VCInstallDir = "C:\\Program Files (x86)\\Microsoft Visual Studio 10.0\\VC\\";
    String const FrameworkSdkDir = "C:\\Program Files (x86)\\Microsoft SDKs\\Windows\\v7.0A\\";
    auto reniRoot = File(__FILE__ "\\..\\..\\..\\").FullName;
    auto cc = VCInstallDir + "bin\\CL.exe";

    auto includeDirs = _({ VCInstallDir + "include" });

    auto libraries = _({
        VCInstallDir + "lib",
        VCInstallDir + "atlmfc\\lib",
        FrameworkSdkDir + "lib"
    });

    auto myLibs = _({
        "C:\\data\\develop\\Reni3\\out\\Debug\\HWLib.lib",
        "C:\\data\\develop\\Reni3\\out\\Debug\\HWLang.lib",
        "C:\\data\\develop\\Reni3\\out\\Debug\\reni.lib"
    });

    Array<String> forcedIncludes;

    auto args = "\"" + cc + "\" /nologo /W4 /WX- /MP /Od /Oy- /D WIN32 /D _DEBUG /D _CONSOLE "
        "/GF- /Gm- /EHsc /RTC1 /MDd /GS /Gy- /fp:precise /Zc:forScope- /GR "
        "/Fo\"C:\\data\\develop\\Reni3\\out\\Debug\\{0}.obj\" "
        "/Gd /TP /FC /errorReport:prompt "
        ;
    for (auto i = 0; i < includeDirs.Count; i++)
        args += "/I" + includeDirs[i].Quote + " ";
    for (auto i = 0; i < forcedIncludes.Count; i++)
        args += "/FI" + forcedIncludes[i].Quote + " ";
    args += "{0} ";
    args += "/link /DEBUG /SUBSYSTEM:CONSOLE ";
    for (auto i = 0; i < libraries.Count; i++)
        args += "/LIBPATH:" + libraries[i].Quote + " ";
    //	args += "  \"kernel32.lib\" \"user32.lib\" \"gdi32.lib\" \"winspool.lib\" \"comdlg32.lib\" \"advapi32.lib\" \"shell32.lib\" \"ole32.lib\" \"oleaut32.lib\" \"uuid.lib\" \"odbc32.lib\" \"odbccp32.lib\" "; 
    args += "  ";
    args += myLibs
        .Select<String>([](String x){return x.Quote + " "; })
        ->Stringify("");

    auto fileName = String("");
    args = args.Replace("{0}", fileName);
    dd("args=" + args);

    //vardump(cpp.AbsolutePathName).println();

    Process ccc(args);
    auto compileResult = ccc.data;
    auto error = ccc.errorData;
    if ((compileResult == "" || compileResult == fileName + "\r\n") && error == ""){
        auto result = Process(fileName + ".exe").data;
        d(result);
        b_;
    };

    dd(compileResult);
    dd(error);
    b_;
};
