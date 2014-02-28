#pragma once
#include "..\HWLib\DumpableObject.h"
#include "..\Runtime\Common.h"
#include "..\HWLib\Ref.h"

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
        static Ref<CodeItem> const Const(Ref<BitsConst> const&value);
        virtual String const ToCpp(CodeVisitor const& visitor)const;
        virtual_p(Size, size) = 0;
    };
}

