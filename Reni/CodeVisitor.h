#pragma once
#include "..\HWLib\DumpableObject.h"

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
    };

}