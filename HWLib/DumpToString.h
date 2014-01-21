#pragma once

#include "String.h"

namespace HWLib
{
    template<typename T>
    String const DumpToString(T const&) { return String(typeid(T).name()); }
    template<typename T>
    String const DumpToString(T const&target, int radix) = delete;
    template<>
    inline String const DumpToString<int>(int const&target) { return String::Convert(target); }
}

using namespace HWLib;

template<typename T>
String const Box_<T>::DumpToString()const{ return HWLib::DumpToString(_data); }

