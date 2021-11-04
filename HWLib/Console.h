#pragma once
#include "Properties.h"

namespace HWLib
{
    template<typename T> class LevelValue;

    class Console  final
    {
        bool _isLineStart;
    public:
        int IndentCount;
        Console();
        void Write(std::string const& text, bool isLine = false);
        void WriteLine(std::string const&text){ Write(text, true); };
        bool BreakTrace(const std::string& condition, const std::string& fileName, int line, const std::string& data);
        template <typename T>
        void FunctionTrace(std::string const&function, T const& object);

        p_nonconst(LevelValue<int>, IndentLevel);
        p(bool, IsDebuggerPresent);
        static Console Instance;
    };
}

#define c_ ::HWLib::Console::Instance

using namespace HWLib;

template <typename T>
void Console::FunctionTrace(std::string const&function, T const& object)
{
    Write(function);
    if (typeid(object) != typeid(T))
    {
        auto header
            = (" *** ")
            + DumpTypeName(object)
            + " *** ";
        Write(header);
    }

    IndentCount++;
    Write(("[ ") + HWLib::Dump(object) + " ]\n");
};

