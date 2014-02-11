#pragma once

namespace HWLib
{
    class String;
    class DumpableObject;

    template <typename T>
    String const DumpToString(T const&target);

    template <>
    String const DumpToString(DumpableObject const&target);
    template <>
    String const DumpToString(int const&target);
    template <>
    String const DumpToString(String const&target);


    template <typename T>
    String const TypeName(T const& object);

}
    