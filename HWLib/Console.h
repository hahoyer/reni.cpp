#pragma once
#include "Properties.h"

namespace HWLib
{
    class String;
    template<typename T> class LevelValue;

    class Console  final
    {
        bool _isLineStart;
    public:
        int IndentCount;
        Console();
        void Write(String const&text, bool isLine = false);
        void WriteLine(String const&text){ Write(text, true); };
        bool BreakTrace(const String&condtion, const String&fileName, int line, const String&data);
        template <typename T>
        void FunctionTrace(String const&function, T const& object);

        p_nonconst(LevelValue<int>, IndentLevel);
        p(bool, IsDebuggerPresent);
        static Console Instance;
    };
}

#define c_ ::HWLib::Console::Instance

using namespace HWLib;

template <typename T>
void Console::FunctionTrace(String const&function, T const& object)
{
    Write(function);
    if (typeid(object) != typeid(T))
    {
        auto header
            = String(" *** ")
            + DumpTypeName(object)
            + " *** ";
        Write(header);
    }

    IndentCount++;
    Write(String("[ ") + HWLib::Dump(object) + " ]\n");
};

