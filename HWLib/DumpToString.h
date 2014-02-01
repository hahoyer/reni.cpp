#pragma once

namespace HWLib
{
    class String;
    class DumpableObject;

    String const DumpToString(DumpableObject const&target);
    String const DumpToString(int const&target);
    String const DumpToString(String const&target);

}
    