#pragma once

namespace Reni
{
    class Type: public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = Type;

    private:
        override_p_function(Array<String>, DumpData){ return{}; };
    };
}
