#include "Import.h"
#include "Print.h"

#include "File.h"
#include "String.h"


namespace HWLib
{

    CString const FilePosition(CString const&FileName, int Line, int Col, CString const&FunctionName)
    {
        return CString("").FilePosition(FileName, Line, Col, FunctionName);
    };


    void Print(const CString&text)
    {
        static auto Active = false;
        if (Active)
            return;

        Active = true;

        auto fff = CFile("..\\log\\print.log");
                                                       
        static auto Open = false;
        if (!Open)
        {
            Open = true;

            CString("\n").OutputDebug();
            FilePosition
                (
                fff.AbsolutePathName,
                fff.String.Count([=](char const c){return c == '\n'; }) + 1, 1,
                "see also"
                )
                .OutputDebug();
            CString("\n").OutputDebug();
        };

        text.OutputDebug();          
        fff.String = text;                           
                         
        Active = false;                    
    };
}
