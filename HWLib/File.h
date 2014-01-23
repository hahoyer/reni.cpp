#pragma once

#include "Properties.h"
#include "Ref.h"

namespace HWLib
{
    class String;

    class File
    {
        class internal;
        Var<internal> _internal;
    public:
        File(String const&name);

        p(String, FullName);
        p_mutable(String, Name);
        p_mutable(String, Data);
    };

}
                           