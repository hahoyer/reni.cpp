#pragma once

namespace Reni
{
    class Type: public WithId<DumpableObject, Type>
    {
        using baseType = WithId<DumpableObject, Type>;
        using thisType = Type;

    private:
        override_p_function(Array<String>, DumpData){ return{}; };
    };
}
