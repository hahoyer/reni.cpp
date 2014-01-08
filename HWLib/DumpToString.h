#pragma once

namespace HWLib
{
    template<typename T>
    String const DumpToString(T const&target);
    template<typename T>
    String const DumpToString(T const&target, int radix);
}
