#include "Import.h"
#include "CodeItem.h"

#include "../HWLib/RefCountContainer.instance.h"

#include "Address.h"
#include "CodeItems.h"
#include "Context.h"
#include "External.h"
#include "Closure.h"
#include "NumberType.h"
#include "ReplaceVisitor.h"
#include "ReferenceCode.h"


using namespace Reni;
using namespace HWLib;

static bool Trace = true;

p_virtual_header_implementation(CodeItem, Size, size);
p_virtual_header_implementation(CodeItem, bool, isReference);
p_virtual_header_implementation(CodeItem, Closure, closure);
p_virtual_header_implementation(CodeItem, bool, isEmpty);

bool CodeItem::operator==(const thisType& other) const
{
  return this == &other;
}

Ref<CodeItem> CodeItem::Assign(const Type& data, Ref<CodeItem> left)
{
  return new FiberConnector({left, Arg(data)}, new AssignCode(data.size));
}

Ref<CodeItem> CodeItem::Arg(const Type& value)
{
  return new ArgCode(value);
}

Ref<CodeItem> CodeItem::NumberOperation(
  string name,
  const Address& result,
  const Type& left,
  const Type& right
)
{
  auto leftAddress = left.toAddress;
  auto rightAddress = right.toAddress;
  auto action = new NumberOperationCode(
    name,
    result.data,
    leftAddress.data,
    leftAddress.depth,
    rightAddress.data,
    rightAddress.depth
  );

  return new FiberConnector(
    {
      This(left),
      Arg(right)
    },
    action
  );
};

Ref<CodeItem> CodeItem::NumberConversion(const Address& result, const Type& arg)
{
  const auto argAddress = arg.toAddress;
  auto action = new NumberConversionCode(
    result.data,
    argAddress.data,
    argAddress.depth
  );

  return (*Arg(arg) + *action)
    ->thisRef;
}

Ref<CodeItem> CodeItem::IfThenElse(const Ref<CodeItem> condition, const Ref<CodeItem> then, const Ref<CodeItem> _else)
{
  fd(condition, then, _else);
  mb;
  return Empty();
}

Ref<CodeItem> CodeItem::CallGetter(const Size& size, const size_t index, const Type& arg)
{
  return (*Arg(arg) + *new CallGetterFiber(size, index, arg.size))
    ->thisRef;
}

Ref<CodeItem> CodeItem::CallGetter(const Size& size, const size_t index)
{
  return (*Empty() + *new CallGetterFiber(size, index, Size(0)))
    ->thisRef;
}

Ref<CodeItem> CodeItem::SmartList(const Array<Optional<Ref<CodeItem>>>& items)
{
  using resultType = Ref<CodeItem>;
  using itemType = decltype(items[0]);

  const auto realItems = items
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

Ref<CodeItem> CodeItem::List(const Array<Ref<CodeItem>>& items)
{
  using resultType = Ref<CodeItem>;

  switch(items.Count)
  {
  case 0:
    return Empty();
  case 1:
    return items[0];
  default:
    fd(items);
    b_;
    return Empty();
  }
}

Ref<CodeItem> CodeItem::Const(const BitsConst& value)
{
  return new ConstCode(value.size, value);
};

Ref<CodeItem> CodeItem::DumpPrint(const NumberType& value)
{
  return (*This(value) + *new DumpPrintNumberCode(value.size))
    ->thisRef;
};

Ref<CodeItem> CodeItem::Reference(const ContainerContext& value)
{
  return new ReferenceCode(value);
}

Ref<CodeItem> CodeItem::This(const Type& value)
{
  return new ThisCode(value);
}

Optional<Ref<CodeItem>> CodeItem::Replace(const ReplaceVisitor& arg) const
{
  const bool Trace = arg.Trace || ObjectId == -16;
  md(closure);
  b_if_(Trace);
  auto result = ReplaceImpl(arg);
  a_if(result.IsEmpty || result.Value->size == size,
       Dump + "\n" +
       nd(result) + "\n" +
       nd(size) + "\n" +
       nd(result.Value->size));
  return_d(result);
}

Ref<FiberCode> CodeItem::operator+(const FiberItem& item) const
{
  const Array<Ref<FiberItem>> items{Ref<FiberItem>(item.thisRef)};
  return thisRef + items;
};

Ref<FiberCode> CodeItem::operator+(const Array<Ref<FiberItem>>& items) const
{
  return new FiberCode(thisRef, items);
};

Ref<CodeItem> CodeItem::ReferencePlus(Size offset) const
{
  md(offset);
  b_;
  return thisRef;
}

Ref<CodeItem> CodeItem::Convert(const Type& type) const
{
  if(size == type.size)
    return thisRef;
  md(type);
  b_;
  return thisRef;
}

Ref<CodeItem> CodeItem::FunctionArg(const Type& value)
{
  if(value.size == 0)
    return Const(BitsConst::Empty());
  return new FunctionArgCode(value);
}

string CodeItem::ToCpp(const CodeVisitor& visitor) const
{
  md(visitor);
  mb;
};


string ConstCode::ToCpp(const CodeVisitor& visitor) const
{
  return visitor.Const(size, value);
};

p_implementation(ConstCode, bool, isEmpty) { return value.isEmpty; }


string DumpPrintNumberCode::ToCpp(const CodeVisitor& visitor) const
{
  return visitor.DumpPrintNumber(_size);
};

TypedCode::TypedCode(const Type& type): type(type)
{}

p_implementation(TypedCode, Array<string>, DumpData) { return {nd(type)}; };
p_implementation(TypedCode, Size, size) { return type.size; }


ArgCode::ArgCode(const Type& type) : baseType(type)
{
  SetDumpString();
  b_if(ObjectId == -10, Dump);
}

p_implementation(ArgCode, Closure, closure) { return Closure(External::Args::Instance); }

string ArgCode::ToCpp(const CodeVisitor& visitor) const
{
  md(visitor);
  mb;
}

Optional<Ref<CodeItem>> ArgCode::ReplaceImpl(const ReplaceVisitor& visitor) const
{
  visitor.Assume(External::Args::Instance, type);
  return visitor.GetCode(External::Args::Instance);
}

Ref<CodeItem> ArgCode::Convert(const Type& type) const
{
  if(type == this->type)
    return thisRef;

  return (thisRef + this->type.ConvertFiber(type))->thisRef;
};


FunctionArgCode::FunctionArgCode(const Type& type)
  : baseType(type.indirectType->thisRef)
{
  SetDumpString();
  b_if(ObjectId == -10, Dump);
}

string FunctionArgCode::ToCpp(const CodeVisitor& visitor) const
{
  return visitor.FunctionArg();
}

ThisCode::ThisCode(const Type& type): baseType(type)
{
  SetDumpString();
}

p_implementation(ThisCode, Closure, closure) { return Closure(External::This::Instance); }

string ThisCode::ToCpp(const CodeVisitor& visitor) const
{
  md(visitor);
  mb;
}

string CallGetterFiber::ToCpp(const CodeVisitor& visitor) const
{
  return visitor.CallGetter(result, index, arg);
}

Optional<Ref<CodeItem>> ThisCode::ReplaceImpl(const ReplaceVisitor& visitor) const
{
  auto rawResult = visitor.GetCode(External::This::Instance);
  if(rawResult.IsEmpty)
    return {};
  const auto Trace = visitor.Trace;
  md(visitor, rawResult);
  auto result = rawResult.Value->Convert(type);
  d(result);
  return result;
};


std::string NumberOperationCode::ToCpp(const CodeVisitor& visitor) const
{
  return visitor.NumberOperation(name, size, leftDepth, leftSize, rightDepth, rightSize);
}


string NumberConversionCode::ToCpp(const CodeVisitor& visitor) const
{
  return visitor.NumberConversion(size, argDepth, _argSize);
}


FiberConnector::FiberConnector(const Array<Ref<CodeItem>>& items, const Ref<FiberConnectorItem>& connector)
  : items(items)
    , connector(connector)
{
  SetDumpString();
  a_if(IsValid, Dump);
}

p_implementation(FiberConnector, Closure, closure)
{
  return items
    .Aggregate<Closure>
    (
      [&](Closure result, Ref<CodeItem> item)
      {
        return result + item->closure;
      }
    )
    + connector->closure;
}

string FiberConnector::ToCpp(const CodeVisitor& visitor) const
{
  return visitor.FiberConnection(items, connector);
}


Optional<Ref<CodeItem>> FiberConnector::ReplaceImpl(const ReplaceVisitor& visitor) const
{
  const bool Trace = visitor.Trace && ObjectId == 6;
  md(visitor);
  const auto replacedItems = items
                             .Select<Optional<Ref<CodeItem>>>
                             (
                               [&](Ref<CodeItem> item) { return item->Replace(visitor); }
                             )
                             ->ToArray;

  d(replacedItems);

  if(
    !replacedItems.Where
    (
      [](Optional<Ref<CodeItem>> item) { return !item.IsEmpty; }
    )->Any
  )
    return_db(Optional<Ref<CodeItem>> {});

  auto index = 0;
  const auto newItems = replacedItems
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


DereferenceCode::DereferenceCode(const Type& type) : baseType(type)
{
  SetDumpString();
}

string DereferenceCode::ToCpp(const CodeVisitor& visitor) const
{
  md(visitor);
  mb;
}
