#pragma once

#include "Properties.h"

namespace HWLib
{
    class CString;

    class CFile
    {
    public:
        CFile(CString const&name);

        GET(CString, AbsolutePathName);
        GETSET(CString, String);
    };

}

