#pragma once

namespace Reni
{
    class BitsConst;

    class CodeItem : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = CodeItem;
    public:
        static Ref<CodeItem> const Const(Ref<BitsConst> const&value);
        virtual_p(String, ToCpp) = 0;
    };
}

