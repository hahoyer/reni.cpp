#pragma once
//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")

using namespace HWLib;

template<typename T>
String const HWLib::DumpToString(T const&) { return String(typeid(T).name()); }
template<typename T>
String const HWLib::DumpToString(T const&target, int radix) = delete;
template<typename T>
String const Box_<T>::DumpToString()const{ return HWLib::DumpToString(_data); }
template<>
inline String const HWLib::DumpToString<int>(int const&target) { return String::Convert(target); }

//#pragma message(__FILE__ "(" STRING(__LINE__) "): ")
