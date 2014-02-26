#pragma once
#include "..\HWLib\DumpableObject.h"
#include "..\HWLib\Ref.h"

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
        virtual String Const(Size const size, Ref<BitsConst> const value) const;
    };

}