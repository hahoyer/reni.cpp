#pragma once

#include "Properties.h"

namespace HWLib
{
    class String;

    class File
    {
        class local;
        local& _data;
    public:
        File(String const&name);
        ~File();

        p(String, FullName);
        p_mutable(String, Name);
        p_mutable(String, Data);
    };

}
                           