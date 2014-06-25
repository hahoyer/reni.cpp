#include "Import.h"
#include "CodeItem.h"

#include "../HWLib/RefCountContainer.instance.h"

#include "CodeItems.h"
#include "External.h"
#include "Externals.h"
#include "NumberType.h"
#include "ReplaceVisitor.h"


using namespace Reni;
using namespace HWLib;

static bool Trace = true;


pure_p_implementation(CodeItem, Size, size);
pure_p_implementation(CodeItem, Externals, exts);
pure_p_implementation(CodeItem, bool, isEmpty);

bool const CodeItem::operator==(thisType const& other) const
{
    return this == &other;
}

Ref<CodeItem> const CodeItem::Arg(Type const&value, int depth)
{
    a_if(*value.dereferencedType == value, nd(value));
    a_if(depth == 0 || value.size > Size(0), nd(value) + nd(depth));
    if(value.size == 0)
        return Const(BitsConst::Empty());
    return new ArgCode(value, depth);
};

Ref<CodeItem> const CodeItem::BinaryOperation(
    String name,
    NumberType const&result,
    NumberType const&left, int leftDepth,
    NumberType const&right, int rightDepth
    )
{
    auto action = new BinaryOperationCode(name, result.size, left.size, leftDepth, right.size, rightDepth);
    return new FiberConnector({This(left, leftDepth), Arg(right, rightDepth)}, action);
};

Ref<CodeItem> const CodeItem::CallGetter(Size const size, int const index, Type const& arg)
{
    return Arg(*arg.dereferencedType, arg.addressLevel)
        ->Fiber({new CallGetterFiber(size, index, arg.size)})
        ->thisRef;
}

Ref<CodeItem> const CodeItem::SmartList(Array<Optional<Ref<CodeItem>>> const& items)
{
    using resultType = Ref<CodeItem>;
    using itemType = decltype(items[0]);

    auto realItems = items
        .Where([&](itemType item)
            {
                return item.IsValid;
            })
        ->Select<resultType>([&](itemType item)
            {
                return item.Value;
            })
        ->ToArray;
    return List(realItems);
}

Ref<CodeItem> const CodeItem::List(Array<Ref<CodeItem>> const& items)
{
    using resultType = Ref<CodeItem>;

    switch(items.Count)
    {
    case 0:
        return Empty();
    case 1:
        return items[0];
    }

    fd(items);
    b_;
    return Empty();
}

Ref<CodeItem> const CodeItem::CallGetter(Size const size, int const index)
{
    fd(size, index);
    b_;
    return Empty();
}

Ref<CodeItem> const CodeItem::Const(BitsConst const&value)
{
    return new ConstCode(value.size, value);
};

Ref<CodeItem> const CodeItem::DumpPrint(NumberType const&value)
{
    return This(value, 0)
        ->Fiber({new DumpPrintNumberCode(value.size)})
        ->thisRef;
};

Ref<CodeItem> const CodeItem::Reference(Type const&value)
{
    return new ReferenceCode(value);
}

Ref<CodeItem> const CodeItem::This(Type const&value, int depth)
{
    if(value.size == 0)
        return Const(BitsConst::Empty());
    return new ThisCode(value, depth);
}

Ref<FiberCode> const CodeItem::Fiber(Array<Ref<FiberItem>> const&items)const
{
    return new FiberCode(thisRef, items);
}

Optional<Ref<CodeItem>> const CodeItem::Replace(ReplaceVisitor const&arg) const
{
    bool Trace = arg.Trace || ObjectId == -5;
    md_;
    auto result = ReplaceImpl(arg);
    a_if(result.IsEmpty || result.Value->size == size, Dump + "\n" + result.Value->Dump);
    return_d(result);
};

Ref<CodeItem> const CodeItem::ReferencePlus(Size offset) const
{
    md(offset);
    b_;
    return thisRef;
}

String const CodeItem::ToCpp(CodeVisitor const& visitor)const
{
    md(visitor);
    mb;
};


String const ConstCode::ToCpp(CodeVisitor const& visitor)const
{
    return visitor.Const(size, value);
};

p_implementation(ConstCode, bool, isEmpty){return value.isEmpty;}


String const DumpPrintNumberCode::ToCpp(CodeVisitor const& visitor)const
{
    return visitor.DumpPrintNumber(_size);
};


p_implementation(TypedCode, Size, size)
{
    if(depth > 0)
        return Size::Address;
    return type.size;
}


p_implementation(ArgCode, Externals, exts){return Externals(External::Arg::Instance);}

String const ArgCode::ToCpp(CodeVisitor const& visitor)const
{
    md(visitor);
    mb;
}

ArgCode::ArgCode(Type const& type, int depth): baseType(type,depth)
{
    SetDumpString();
    b_if(ObjectId == -10, Dump);
}

Optional<Ref<CodeItem>> const ArgCode::ReplaceImpl(ReplaceVisitor const&visitor) const
{
    return visitor.Arg(type, depth);
};


p_implementation(ThisCode, Externals, exts){return Externals(External::This::Instance);}

String const ThisCode::ToCpp(CodeVisitor const& visitor)const
{
    md(visitor);
    mb;
}

String const CallGetterFiber::ToCpp(CodeVisitor const& visitor)const
{
    return visitor.CallGetter(result, index, arg);
}

Optional<Ref<CodeItem>> const ThisCode::ReplaceImpl(ReplaceVisitor const&visitor) const
{
    return visitor.This(type, depth);
};


String const BinaryOperationCode::ToCpp(CodeVisitor const& visitor)const
{
    return visitor.BinaryOperation(name, size, leftDepth, leftSize, rightDepth, rightSize);
}


FiberConnector::FiberConnector(Array<Ref<CodeItem>> const&items, Ref<FiberConnectorItem> const&connector)
    : items(items)
      , connector(connector)
{
    SetDumpString();
    a_if(IsValid, Dump);
}

p_implementation(FiberConnector, Externals, exts)
{
    return items
        .Aggregate<Externals>
        (
            [&](Externals result, Ref<CodeItem> item)
            {
        return result + item->exts;
            }
        )
        + connector->exts;
}

String const FiberConnector::ToCpp(CodeVisitor const& visitor)const
{
    return visitor.FiberConnection(items, connector);
}


Optional<Ref<CodeItem>> const FiberConnector::ReplaceImpl(ReplaceVisitor const&visitor) const
{
    bool Trace = visitor.Trace && ObjectId == 6;
    md(visitor);
    auto replacedItems = items
        .Select<Optional<Ref<CodeItem>>>
        (
            [&](Ref<CodeItem> item){return item->Replace(visitor);}
        )
        ->ToArray;

    d(replacedItems);

    if(
        !replacedItems.Where
        (
            [](Optional<Ref<CodeItem>> item){return !item.IsEmpty;}
        )->Any
    )
    return_db(Optional<Ref<CodeItem>> {});

    auto index = 0;
    auto newItems = replacedItems
        .Select<Ref<CodeItem>>
        (
            [&](Optional<Ref<CodeItem>> item)
            {
                return item || items[index++];
            }
        )
        ->ToArray;

    return_db(new FiberConnector(newItems COMMA connector));
};


p_implementation(ReferenceCode, Externals, exts)
{
    md_;
    mb;
}

inline Ref<CodeItem> const ReferenceCode::ReferencePlus(Size offset) const
{
    if(offset == 0)
        return thisRef;
    return Fiber({new ReferencePlusCode(size, offset)})
        ->thisRef;
}

Optional<Ref<CodeItem>> const ReferenceCode::ReplaceImpl(ReplaceVisitor const&arg) const
{
    md(arg);
    mb;
}

