#include "Import.h"
#include "Print.h"

#include <windows.h>
#include "File.h"
#include "String.h"


namespace HWLib
{

    String const FilePosition(String const&FileName, int Line, int Col, String const&FunctionName)
    {
        return String("").FilePosition(FileName, Line, Col, FunctionName);
    };


    void OutputDebug(const String&text){::OutputDebugStringA(text.Data); }

    void Print(const String&text)
    {
        static auto Active = false;
        if (Active)
            return;

        Active = true;

        auto fff = File("..\\log\\print.log");
                                                       
        static auto Open = false;
        if (!Open)
        {
            Open = true;

            OutputDebug("\n");
            OutputDebug(FilePosition
                (
                fff.FullName,
                fff.Data.Count([=](char const c){return c == '\n'; }) + 1, 1,
                "see also"
                ));
            OutputDebug("\n");
        };

        OutputDebug(text);          
        fff.Data = text;                           
                         
        Active = false;                    
    };
}
