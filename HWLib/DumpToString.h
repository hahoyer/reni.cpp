#pragma once

namespace HWLib
{
    class String;

    template<typename T>
    String const DumpToString(T const&target);
    template<typename T>
    String const DumpToString(T const&target, int radix);
    template<>
    String const DumpToString<int>(int const&target);

}
    