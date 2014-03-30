#pragma once

namespace HWLib{
    class String;
    struct System{
        static String const EnvironmentVariable  (String const&key);
        static String const FormatLastErrorMessage();
        static String const ModuleName(unsigned __int64 instructionPointer);
        static void        Sleep                (int milliseconds);
    };
};
