#pragma once

#include "Code.h"
#include "Size.h"

namespace Reni{
    class ConstCode final : public CodeItem {
        using baseType = CodeItem;
        using thisType = ConstCode;
    public:
        Size const size;
        Ref<BitsConst> const value;

        ConstCode(Size const&size, Ref<BitsConst> const&value)
            : size(size)
            , value(value)
        {}

    private:
        override_p_function(Array<String>, DumpData){ return{ nd(size), nd(value) }; };
        override_p_function(String, ToCpp);
    };
}