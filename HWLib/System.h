#pragma once

namespace HWLib
{
    class String;

    struct System
    {
        static std::string EnvironmentVariable(String const& key);
        static std::string FormatLastErrorMessage();
        static void ThrowLastErrorMessage();
        static std::string ModuleName(unsigned __int64 instructionPointer);
        static void Sleep(int milliseconds);
        static __int64 Ticks();
        static __int64 TicksPerSecond();
    };
};