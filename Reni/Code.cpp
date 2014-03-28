#include "Import.h"
#include "Code.h"

#include "CodeItems.h"
#include "CodeVisitor.h"
#include "FeatureProvider.h"
#include "NumberType.h"
#include "ReplaceVisitor.h"
#include "Result.h"
#include "../HWLib/RefCountContainer.instance.h"

using namespace Reni;
using namespace HWLib;

static bool Trace = true;


String const CodeItem::ToCpp(CodeVisitor const& visitor)const{
    md(visitor);
    mb;
};

pure_p_implementation(CodeItem, Size, size) ;

Ref<CodeItem> const CodeItem::Const(BitsConst const&value){
    return new ConstCode(value.size, value);
};

Ref<CodeItem> const CodeItem::DumpPrint(NumberType const&value){
    return Arg(value)
        ->Fiber({new DumpPrintNumberCode(value.size)});
};

Ref<CodeItem> const CodeItem::BinaryOperation(String name, NumberType const&result, NumberType const&left, NumberType const&right){
    auto action = new BinaryOperationCode(name, result.size, left.size, right.size);
    return new PairCode(This(left), Arg(right), action);
};

Ref<CodeItem> const CodeItem::Arg(Type const&value){
    return new ArgCode(value);
};

Ref<CodeItem> const CodeItem::This(Type const&value){
    return new ThisCode(value);
};

Ref<CodeItem, true> const CodeItem::Replace(ReplaceVisitor const&arg) const{
    md(arg);
    mb;
};

Ref<CodeItem> const CodeItem::Fiber(Array<Ref<FiberItem>> const&items)const{
    return *Reni::Fiber::Create(thisRef, items);
}


String const ConstCode::ToCpp(CodeVisitor const& visitor)const{
    return visitor.Const(size, value);
};


String const DumpPrintNumberCode::ToCpp(CodeVisitor const& visitor)const{
    return visitor.DumpPrintNumber(size);
};


String const ArgCode::ToCpp(CodeVisitor const& visitor)const{
    md(visitor);
    mb;
}

Ref<CodeItem, true> const ArgCode::Replace(ReplaceVisitor const&visitor) const{
    return visitor.Arg(type);
};


String const ThisCode::ToCpp(CodeVisitor const& visitor)const{
    md(visitor);
    mb;
}

Ref<CodeItem, true> const ThisCode::Replace(ReplaceVisitor const&visitor) const{
    return visitor.This(type);
};


String const BinaryOperationCode::ToCpp(CodeVisitor const& visitor)const{
    return visitor.BinaryOperation(name, size, leftSize, rightSize);
}


PairCode::PairCode(Ref<CodeItem> const&left, Ref<CodeItem> const&right, Ref<FiberConnector> const&connector)
: left(left)
, right(right)
, connector(connector) {
    SetDumpString();
    a_if(IsValid, Dump);
}

String const PairCode::ToCpp(CodeVisitor const& visitor)const {
    return visitor.Pair(left, right, connector);
}

Ref<CodeItem, true> const PairCode::Replace(ReplaceVisitor const&visitor) const{
    auto newLeft = left->Replace(visitor);
    auto newRight = right->Replace(visitor);
    if(newLeft.IsEmpty && newRight.IsEmpty)
        return{};
    return new PairCode(*(newLeft || left),*(newRight|| right), connector);
};


