#pragma once

#include "Properties.h"

namespace HWLib
{
    class CString;

    class CFile
    {
    public:
        CFile(CString const&name);

        p(CString, AbsolutePathName);
        p_mutable(CString, String);
    };

}

