#pragma once
#include "../HWLib/String.h"
#include "../Runtime/Common.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/RefCountProvider.h"
#include "../HWLib/Ref.h"
#include "ArrayType.h"
#include "../Util/BitsConst.h"

using namespace HWLib;

namespace Reni
{
    class CodeVisitor;
    class Externals;
    class FiberItem;
    class NumberType;
    class ReplaceVisitor;
    class Result;

    class CodeItem 
        : public WithId<DumpableObject>
        , public RefCountProvider
    {
        typedef DumpableObject baseType;
        typedef CodeItem thisType;
    public:
        typedef ReniRuntime::Stack::dataType dataItemType;
        typedef ReniRuntime::Stack::sizeType dataItemSizeType;

        static Ref<CodeItem> const Arg(Type const&value, int depth);
        static Ref<CodeItem> const Const(BitsConst const&value);
        static Ref<CodeItem> const BinaryOperation
            (
            String name, 
            NumberType const&result, 
            NumberType const&left, int leftDepth,
            NumberType const&right, int rightDepth
            );
        static Ref<CodeItem> const DumpPrint(NumberType const&value);
        static Ref<CodeItem> const This(Type const&value, int depth);
        static Ref<CodeItem> const Reference(Type const&target);

        bool const operator==(thisType const&other)const;

        Ref<CodeItem> const CallGetter(Size const size, int const index) const;

        virtual String const ToCpp(CodeVisitor const& visitor)const;
        virtual_p(Size, size) = 0;
        virtual_p(Externals, externals) = 0;
        ThisRef;
        Optional<Ref<CodeItem>> const Replace(ReplaceVisitor const&arg) const;
        virtual Ref<CodeItem> const Fiber(Array<Ref<FiberItem>> const&items)const;
        virtual Ref<CodeItem> const ReferencePlus(Size offset)const;
    protected:
        virtual Optional<Ref<CodeItem>> const ReplaceImpl(ReplaceVisitor const&arg) const = 0;
    };
}

