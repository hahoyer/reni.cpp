#pragma once

#include "Enumerable.h"
#include "DefaultAssignmentOperator.h"

namespace HWLib
{
    class String: public IEnumerable<char const>
    {
        typedef IEnumerable<char const> base;
        class local;
        local& _data;
    public:
        String(char const* data);
        String(String const& other);
        String(IEnumerable<char const> const& other);

        DefaultAssignmentOperator(String);

        p(char const*, Data);

        void OutputDebug() const;
        String const FilePosition(String const&FileName, int Line, int Col, String const&FunctionName)const;

        String const operator+ (String const& other)const{ return Concat(other); }
    };
}

