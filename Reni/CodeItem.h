#pragma once
#include "../HWLib/String.h"
#include "../Runtime/Common.h"
#include "../HWLib/DumpableObject.h"
#include "../HWLib/RefCountProvider.h"
#include "../HWLib/Ref.h"
#include "ArrayType.h"
#include "../Util/BitsConst.h"

using namespace HWLib;
using namespace Util;

namespace Reni
{
    class ContainerContext;
    class CodeVisitor;
    class Externals;
    class FiberCode;
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

        static Ref<CodeItem> const Assign(Type const&data, Ref<CodeItem> left);
        static Ref<CodeItem> const Arg(Type const&type);
        static Ref<CodeItem> const Dereference(Type const&type);
        static Ref<CodeItem> const FunctionArg(Type const&value);
        static Ref<CodeItem> const Const(BitsConst const&value);
        static Ref<CodeItem> const Empty(){ return Const(BitsConst::Empty()); };
        static Ref<CodeItem> const NumberOperation
            (
            String name, 
            Address const&result, 
            Type const&left,
            Type const&right
            );
        static Ref<CodeItem> const DumpPrint(NumberType const&value);
        static Ref<CodeItem> const This(Type const&type);
        static Ref<CodeItem> const Reference(ContainerContext const&value);
        static Ref<CodeItem> const CallGetter(Size const& size, int const index, Type const&arg);
        static Ref<CodeItem> const CallGetter(Size const& size, int const index);
        static Ref<CodeItem> const SmartList(Array<Optional<Ref<CodeItem>>> const&items);
        static Ref<CodeItem> const List(Array<Ref<CodeItem>> const&items);
        static Ref<CodeItem> const NumberConversion(Address const&result, Type const& arg);
        static Ref<CodeItem> const IfThenElse(Ref<CodeItem> const condition, Ref<CodeItem> const then, Ref<CodeItem> const _else);

        bool const operator==(thisType const&other)const;

        virtual String const ToCpp(CodeVisitor const& visitor)const;
        virtual_p(bool, isReference){ return false; };
        virtual_p(Size, size) = 0;
        virtual_p(Externals, exts) = 0;
        virtual_p(bool, isEmpty){ return false; };
        ThisRef;
        Optional<Ref<CodeItem>> const Replace(ReplaceVisitor const&arg) const;
        virtual Ref<FiberCode> const Fiber(Array<Ref<FiberItem>> const&items)const;
        virtual Ref<CodeItem> const ReferencePlus(Size offset)const;
    protected:
        virtual Optional<Ref<CodeItem>> const ReplaceImpl(ReplaceVisitor const&arg) const = 0;
    };
}

