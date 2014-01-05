#pragma once

#include "Enumerable.h"
#include "DefaultAssignmentOperator.h"

namespace HWLib
{
    class CString: public IEnumerable<char const>
    {
        char* const _data;
    public:
        CString(const char* data);
        CString(const CString& other);
        DefaultAssignmentOperator(CString);
        void OutputDebug() const;
        CString const FilePosition(CString const&FileName, int Line, int Col, CString const&FunctionName)const;
    };
}

