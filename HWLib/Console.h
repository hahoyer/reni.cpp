#pragma once
#include "Properties.h"

namespace HWLib
{
    class String;

    class Console  final
    {
        bool _isLineStart;
    public:
        int IndentLevel;
        Console();
        void Write(String const&text, bool isLine = false);
        void WriteLine(String const&text){ Write(text, true); };
        bool BreakTrace(const String&condtion, const String&fileName, int line, const String&data);
        void FunctionTrace(String const&function);
        template <typename T>
        void FunctionTrace(String const&function, T const& object);

        p(bool, IsDebuggerPresent);
        static Console Instance;
    };
}

#define _console_ ::HWLib::Console::Instance.

using namespace HWLib;

template <typename T>
void Console::FunctionTrace(String const&function, T const& object)
{
    Write(function);
    if (typeid(object) != typeid(T))
    {
        auto header
            = String(" *** ")
            + TypeName(object)
            + " *** ";
        Write(header);
    }

    IndentLevel++;
    Write(String("[ ") + DumpToString(object) + " ]\n");
};

