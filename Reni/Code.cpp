#include "Import.h"
#include "Code.h"

#include "../HWLib/RefCountContainer.instance.h"

#include "CodeItems.h"
#include "External.h"
#include "Externals.h"
#include "NumberType.h"
#include "ReplaceVisitor.h"


using namespace Reni;
using namespace HWLib;

static bool Trace = true;


Ref<CodeItem> const CodeItem::Reference(Type const&value)
{
    return new ReferenceCode(value);
}

String const CodeItem::ToCpp(CodeVisitor const& visitor)const
{
    md(visitor);
    mb;
};

pure_p_implementation(CodeItem, Size, size) ;

pure_p_implementation(CodeItem, Externals, externals) ;

Ref<CodeItem> const CodeItem::Const(BitsConst const&value)
{
    return new ConstCode(value.size, value);
};

Ref<CodeItem> const CodeItem::DumpPrint(NumberType const&value)
{
    return This(value,0)
        ->Fiber({new DumpPrintNumberCode(value.size)});
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

Ref<CodeItem> const CodeItem::Arg(Type const&value, int depth)
{
    if(value.size == 0)
        return Const(BitsConst::Empty());
    return new ArgCode(value,depth);
};

Ref<CodeItem> const CodeItem::This(Type const&value, int depth)
{
    if(value.size == 0)
        return Const(BitsConst::Empty());
    return new ThisCode(value, depth);
};

Ref<CodeItem, true> const CodeItem::Replace(ReplaceVisitor const&arg) const
{
    bool Trace = arg.Trace;
    md_;
    auto result = ReplaceImpl(arg);
    a_if(result.IsEmpty || result->size == size, Dump + "\n" + result->Dump);
    return_d(result);
};

Ref<CodeItem> const CodeItem::Fiber(Array<Ref<FiberItem>> const&items)const
{
    return *Fiber::Create(thisRef, items);
}

Ref<CodeItem> const CodeItem::ReferencePlus(Size offset) const
{
    md(offset);
    b_;
    return thisRef;
}

String const ConstCode::ToCpp(CodeVisitor const& visitor)const
{
    return visitor.Const(size, value);
};


String const DumpPrintNumberCode::ToCpp(CodeVisitor const& visitor)const
{
    return visitor.DumpPrintNumber(size);
};


p_implementation(TypedCode, Size, size)
{
    if(depth > 0)
        return Size::Address;
    return type.size;
}


p_implementation(ArgCode, Externals, externals)
{
    return External::Arg::Instance;
}

String const ArgCode::ToCpp(CodeVisitor const& visitor)const
{
    md(visitor);
    mb;
}

Ref<CodeItem, true> const ArgCode::ReplaceImpl(ReplaceVisitor const&visitor) const
{
    return visitor.Arg(type, depth);
};


p_implementation(ThisCode, Externals, externals)
{
    return External::This::Instance;
}

String const ThisCode::ToCpp(CodeVisitor const& visitor)const
{
    md(visitor);
    mb;
}

Ref<CodeItem, true> const ThisCode::ReplaceImpl(ReplaceVisitor const&visitor) const
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

p_implementation(FiberConnector, Externals, externals)
{
    return items
        .Aggregate<Externals>
        (
            [&](Externals result, Ref<CodeItem> item)
            {
                return result + item->externals;
            }
        )
        + connector->externals;
}

String const FiberConnector::ToCpp(CodeVisitor const& visitor)const
{
    return visitor.FiberConnection(items, connector);
}


Ref<CodeItem, true> const FiberConnector::ReplaceImpl(ReplaceVisitor const&visitor) const
{
    bool Trace = visitor.Trace && ObjectId == 6;
    md(visitor);
    auto replacedItems = items
        .Select<Ref<CodeItem,true>>
        (
            [&](Ref<CodeItem> item){return item->Replace(visitor);}
        )
        ->ToArray;

    d(replacedItems);

    if(
        !replacedItems.Where
        (
            [](Ref<CodeItem, true> item){return !item.IsEmpty;}
        )->Any
    )
        return_db(Ref<CodeItem COMMA true> {});

    auto index = 0;
    auto newItems = replacedItems
        .Select<Ref<CodeItem>>
        (
            [&](Ref<CodeItem,true> item)
            {
                return item || items[index++];
            }
        )
        ->ToArray;

    return_db(new FiberConnector(newItems COMMA connector));
};


p_implementation(ReferenceCode, Externals, externals)
{
    md_;
    mb;
}

inline Ref<CodeItem> const ReferenceCode::ReferencePlus(Size offset) const
{
    if(offset == 0)
        return thisRef;
    return Fiber({new ReferencePlusCode(size, offset)});
}

Ref<CodeItem, true> const ReferenceCode::ReplaceImpl(ReplaceVisitor const&arg) const
{
    md(arg);
    mb;
}

p_implementation(FiberItem, Externals, externals)
{
    return{};
}

p_implementation(FiberConnectorItem, Externals, externals)
{
    return{};
}

p_implementation(ConstCode, Externals, externals)
{
    return{};
}

p_implementation(DumpPrintNumberCode, Externals, externals)
{
    return{};
}

p_implementation(ReferencePlusCode, Externals, externals)
{
    return{};
}

p_implementation(BinaryOperationCode, Externals, externals)
{
    return{};
}