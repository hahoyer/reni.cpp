#pragma once
#include "..\HWLib\DumpableObject.h"
#include "Code.h"

using namespace HWLib;

namespace Reni
{
    class Size;
    class BitsConst;

    class CodeVisitor: public DumpableObject
    {
        using thisType = CodeVisitor;
        using baseType = DumpableObject;

    public:
        virtual String const Const(Size const size, BitsConst const& value) const;
        virtual String const DumpPrintNumber(Size const size) const;
        virtual String const Pair(Ref<CodeItem> const&left, Ref<CodeItem> const&right) const;
        virtual String const BinaryOperation(String const& name, Size const&size, Size const&leftSize, Size const&rightSize)const;
    };

}