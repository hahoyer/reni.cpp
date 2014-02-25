#include "Import.h"
#include "CodeView.h"

using namespace Reni;
static bool Trace = true;


CodeView::CodeView(String const& cppCode) : cppCode(cppCode){}

p_implementation(CodeView, String, program)
{
    static String const result = R"(
    #include <stdlib.h>
    #include "Common.h"
    using namespace ReniRuntime; 
    int main(void){
        {0}
        return 22;
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
    if (pcpp.Execute())
    {
        auto compileResult = pcpp.data;
        auto error = pcpp.errorData;
        dd("compileResult: " + compileResult);
        dd("error: " + error);
        b_;

        return;
    }

    Process pexe(exe);
    auto e = pexe.Execute();
    d(pexe.data);
    d(pexe.errorData);
    b_;

};


p_implementation(CodeView, String, CompileCommand){
    return
        String()
        + R"(cl.exe )"
        + R"(/Zi /Zl /nologo /W4 /WX- /sdl /MP8 /Od /Oi /Oy- /D _DEBUG /Gm- /EHsc )"
        + R"(/MDd /GS /Gy- /fp:precise /Zc:wchar_t /Zc:forScope /GR /TP /analyze- /FC /errorReport:prompt )"
        + fullFileName + ".cpp "
        + Includes + " "
        + R"(/link /NOLOGO /DEBUG /OPT:NOREF /MACHINE:X86 )"
        + R"(/LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib" )"
        + R"(/LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\atlmfc\lib" )"
        + R"(/LIBPATH:"C:\Program Files (x86)\Windows Kits\8.1\lib\winv6.3\um\x86" )"
        + R"(/LIBPATH:"C:\Program Files (x86)\boost_1_55_0\libs" )"
        + R"(/LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib" )"
        + R"(/LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\atlmfc\lib" )"
        + R"(/LIBPATH:"C:\Program Files (x86)\Windows Kits\8.1\lib\winv6.3\um\x86" )"
        + R"(/LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib" )"
        //+ R"(a:\develop\C++\Reni\bin\out\Debug\HWLib.lib )"
        //+ R"(a:\develop\C++\Reni\bin\out\Debug\HWLang.lib )"
        //+ R"(a:\develop\C++\Reni\bin\out\Debug\Reni.lib )"
        + R"(MSVCRTD.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib )"
        + "/OUT:\"" + fullFileName + ".exe\" "
        ;
};

p_implementation(CodeView, String, Includes){
    Array<String>list{
        get_VCInstallDir() + "include",
        get_Boost(),
        get_RuntimeDir()
    };

    return list
        .Select<String>([](String dir){return "/I\"" + dir + "\""; })
        ->Stringify(" ");
};

p_implementation(CodeView, String, Libs){
    Array<String>list{
//        "kernel32.lib",
        "user32.lib",
        "gdi32.lib",
        "winspool.lib",
        "comdlg32.lib",
        "advapi32.lib",
        "shell32.lib",
        "ole32.lib",
        "oleaut32.lib",
        "uuid.lib",
        "odbc32.lib",
        "odbccp32.lib"
    };

    return list
        //.Select<String>([&](String name){return "\"" + VCInstallDir + "lib\\" + name + "\""; })
        .Stringify(" ");
};


