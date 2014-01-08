#pragma once

namespace HWLib
{
    class String;

    class Console
    {
        bool _isLineStart;
    public:
        int IndentLevel;
        Console();
        void Write(String const&text, bool isLine = false);
        void WriteLine(String const&text){ Write(text, true); };
        bool BreakTrace(const String&condtion, const String&fileName, int line, const String&data);
        static Console Instance;
    };
}

#define _console_ ::HWLib::Console::Instance.

