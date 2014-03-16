#pragma once
#include "../HWLib/String.h"
#include "../Runtime/Common.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/RefCountProvider.h"
#include "../HWLib/Ref.h"
#include "ArrayType.h"

using namespace HWLib;

namespace Reni
{
    class BitsConst;
    class CodeVisitor;
    class Size;
    class Result;
    class FiberItem;
    class ReplaceVisitor;

    class CodeItem 
        : public DumpableObject
        , public RefCountProvider
    {
        typedef DumpableObject baseType;
        typedef CodeItem thisType;
    public:
        typedef ReniRuntime::Stack::dataType dataItemType;
        typedef ReniRuntime::Stack::sizeType dataItemSizeType;
    public:
        static Ref<CodeItem> const Arg(Type const&value);
        static Ref<CodeItem> const Const(BitsConst const&value);
        static Ref<CodeItem> const DumpPrint(NumberType const&value);
        virtual String const ToCpp(CodeVisitor const& visitor)const;
        virtual_p(Size, size) = 0;
        AssumeConstObject;
        virtual Ref<CodeItem,true> const Replace(ReplaceVisitor const&arg) const;
        virtual Ref<CodeItem> const Fiber(Array<Ref<FiberItem>> const&items)const;
    };
}

