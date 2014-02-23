#pragma once

namespace HWLib{
    class String;
    struct System{
        static String const EnvironmentVariable(String const&key);
        static String const FormatLastErrorMessage();
        static void Sleep(int milliseconds);

    };
};
