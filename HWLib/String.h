#pragma once

#include "Enumerable.h"

namespace HWLib
{
    class CString: public IEnumerable<char const>
    {
        char* const _data;
    public:
        CString(const char* data);
        void OutputDebug() const;
        CString const FilePosition(CString const&FileName, int Line, int Col, CString const&FunctionName)const;
    };
}

