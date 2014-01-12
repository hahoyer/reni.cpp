#pragma once

#include "String.h"

namespace HWLib
{
    template<typename T>
    String const DumpToString(T const&) { return String(typeid(T).name()); }
    template<typename T>
    String const DumpToString(T const&target, int radix) = delete;


}
