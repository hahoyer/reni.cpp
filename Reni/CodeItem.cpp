#include "Import.h"
#include "CodeItem.h"

#include "../HWLib/RefCountContainer.instance.h"

#include "Address.h"
#include "CodeItems.h"
#include "Context.h"
#include "External.h"
#include "Externals.h"
#include "NumberType.h"
#include "ReplaceVisitor.h"
#include "ReferenceCode.h"


using namespace Reni;
using namespace HWLib;

static bool Trace = true;

p_virtual_header_implementation(CodeItem, Size, size);
p_virtual_header_implementation(CodeItem, bool, isReference);
p_virtual_header_implementation(CodeItem, Externals, exts);
p_virtual_header_implementation(CodeItem, bool, isEmpty);

bool const CodeItem::operator==(thisType const& other) const
{
    return this == &other;
}

Ref<CodeItem> const CodeItem::Assign(Type const& data, Ref<CodeItem> left)
{
    return new FiberConnector({left, Arg(data)}, new AssignCode(data.size));
}

Ref<CodeItem> const CodeItem::Arg(Type const&value)
{
    return new ArgCode(value);
}

Ref<CodeItem> const CodeItem::NumberOperation(
    String name,
    Address const&result,
    Type const&left,
    Type const&right
    )
{
    auto leftAddress = left.toAddress;
    auto rightAddress = right.toAddress;
    auto action = new NumberOperationCode
        (
        name, 
        result.data, 
        leftAddress.data, 
        leftAddress.depth, 
        rightAddress.data, 
        rightAddress.depth
        );

    return new FiberConnector
        (
            {
                This(left),
                Arg(right)
            },
            action
        );
};

Ref<CodeItem> const CodeItem::NumberConversion(Address const& result, Type const& arg)
{
    auto argAddress = arg.toAddress;
    auto action = new NumberConversionCode
        (
            result.data,
            argAddress.data,
            argAddress.depth
        );

    return (*Arg(arg) + *action)
        ->thisRef;
}

Ref<CodeItem> const CodeItem::IfThenElse(Ref<CodeItem> const condition, Ref<CodeItem> const then, Ref<CodeItem> const _else)
{
    fd(condition, then, _else);
    mb;
    return Empty();
}

Ref<CodeItem> const CodeItem::CallGetter(Size const &size, int const index, Type const& arg)
{
    return (*Arg(arg) + *new CallGetterFiber(size, index, arg.size))
        ->thisRef;
}

Ref<CodeItem> const CodeItem::CallGetter(Size const &size, int const index)
{
    return (*Empty() + *new CallGetterFiber(size, index, Size(0)))
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

Ref<CodeItem> const CodeItem::Const(BitsConst const&value)
{
    return new ConstCode(value.size, value);
};

Ref<CodeItem> const CodeItem::DumpPrint(NumberType const&value)
{
    return (*This(value) + *new DumpPrintNumberCode(value.size))
        ->thisRef;
};

Ref<CodeItem> const CodeItem::Reference(ContainerContext const&value)
{
    return new ReferenceCode(value);
}

Ref<CodeItem> const CodeItem::This(Type const&value)
{
    return new ThisCode(value);
}

Optional<Ref<CodeItem>> const CodeItem::Replace(ReplaceVisitor const&arg) const
{
    bool Trace = arg.Trace || ObjectId == -16;
    md(exts);
    b_if_(Trace);
    auto result = ReplaceImpl(arg);
    a_if(result.IsEmpty || result.Value->size == size, 
        Dump + "\n" + 
        nd(result) + "\n" + 
        nd(size) + "\n" + 
        nd(result.Value->size));
    return_d(result);
}

Ref<FiberCode> const CodeItem::operator+(FiberItem const& item) const
{
    Array<Ref<FiberItem>> items{Ref<FiberItem>(item.thisRef)};
    return thisRef + items;
};

Ref<FiberCode> const CodeItem::operator+(Array<Ref<FiberItem>> const&items) const
{
    return new FiberCode(thisRef, items);
};

Ref<CodeItem> const CodeItem::ReferencePlus(Size offset) const
{
    md(offset);
    b_;
    return thisRef;
}

Ref<CodeItem> const CodeItem::Convert(Type const& type) const
{
    if(size == type.size)
        return thisRef;
    md(type);
    b_;
    return thisRef;
}

Ref<CodeItem> const CodeItem::FunctionArg(Type const& value)
{
    if (value.size == 0)
        return Const(BitsConst::Empty());
    return new FunctionArgCode(value);
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

TypedCode::TypedCode(Type const& type): type(type)
{
}

p_implementation(TypedCode, Array<String>, DumpData) { return{nd(type)}; };
p_implementation(TypedCode, Size, size){return type.size;}


ArgCode::ArgCode(Type const& type) : baseType(type)
{
    SetDumpString();
    b_if(ObjectId == -10, Dump);
}

p_implementation(ArgCode, Externals, exts){ return Externals(External::Args::Instance); }

String const ArgCode::ToCpp(CodeVisitor const& visitor)const
{
    md(visitor);
    mb;
}

Optional<Ref<CodeItem>> const ArgCode::ReplaceImpl(ReplaceVisitor const&visitor) const
{
    visitor.Assume(External::Args::Instance, type);
    return visitor.GetCode(External::Args::Instance);
}

Ref<CodeItem> const ArgCode::Convert(Type const& type) const
{
    if(type == this->type)
        return thisRef;

    return (thisRef + this->type.ConvertFiber(type))->thisRef;
};


FunctionArgCode::FunctionArgCode(Type const& type) 
    : baseType(type.indirectType->thisRef)
{
    SetDumpString();
    b_if(ObjectId == -10, Dump);
}

String const FunctionArgCode::ToCpp(CodeVisitor const& visitor)const
{
    return visitor.FunctionArg();
}

ThisCode::ThisCode(Type const& type): baseType(type)
{
    SetDumpString();
}

p_implementation(ThisCode, Externals, exts){ return Externals(External::This::Instance); }

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
    auto rawResult = visitor.GetCode(External::This::Instance);
    if(rawResult.IsEmpty)
        return{};
    auto Trace = visitor.Trace;
    md(visitor,rawResult);
    auto result = rawResult.Value->Convert(type);
    d(result);
    return result;
};


String const NumberOperationCode::ToCpp(CodeVisitor const& visitor)const
{
    return visitor.NumberOperation(name, size, leftDepth, leftSize, rightDepth, rightSize);
}


String const NumberConversionCode::ToCpp(CodeVisitor const& visitor) const
{
    return visitor.NumberConversion(size, argDepth, _argSize);
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


DereferenceCode::DereferenceCode(Type const& type) : baseType(type)
{
    SetDumpString();
}

String const DereferenceCode::ToCpp(CodeVisitor const& visitor) const
{
    md(visitor);
    mb;
}

