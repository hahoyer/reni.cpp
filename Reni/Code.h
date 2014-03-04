#pragma once
#include "../HWLib/String.h"
#include "../Runtime/Common.h"

using namespace HWLib;

namespace Reni
{
    class BitsConst;
    class CodeVisitor;
    class Size;

    class CodeItem : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = CodeItem;
    public:
        using dataItemType = ReniRuntime::Stack::dataType;
        using dataItemSizeType = ReniRuntime::Stack::sizeType;
    public:
        static Ref<CodeItem> const Const(BitsConst const&value);
        virtual String const ToCpp(CodeVisitor const& visitor)const;
        virtual_p(Size, size) = 0;
    };
}

