#pragma once

namespace HWLib
{
    class ConsoleType
    {
    public:
        void Write(String const&text)const;
    };

    static ConsoleType Console;
}

