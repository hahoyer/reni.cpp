#include "Import.h"
#include "Fiber.h"

#include "FeatureProvider.h"
#include "Result.h"
#include "../HWLib/RefCountContainer.instance.h"
#include "ReplaceVisitor.h"

using namespace Reni;
using namespace HWLib;

static bool Trace = true;


virtual_p_implementation(FiberItem, Size, argSize){
    return Size(0);
}

virtual_p_implementation(FiberItem, Size, size){
    return Size(0);
}


Ref<FiberItem,true> const FiberItem::Replace(ReplaceVisitor const&visitor) const {
    md(visitor);
    mb;
};


String const FiberItem::ToCpp(CodeVisitor const& visitor)const{
    md(visitor);
    mb;
};


p_implementation(Fiber, Size, size){
    return items.Last->size;
};

p_implementation(Fiber, bool, IsValid) {
    Size size = head->size;
    for(auto item: items){
        if(size != item->argSize)
            return false;
        size = item->size;
    }
    return true;
};

Ref<Fiber> Fiber::Create(Ref<CodeItem> const& head, Array<Ref<FiberItem>> const& items) {
    return new thisType(head, items);
}

Ref<CodeItem, true> const Fiber::Replace(ReplaceVisitor const&visitor) const {
    Ref<CodeItem, true> newHead = head->Replace(visitor);
    Array<Ref<FiberItem,true>> newItems = items
        .Select<Ref<FiberItem, true>>([&](Ref<FiberItem> item) {return item->Replace(visitor); })
        ->ToArray;
    return *ReCreate(newHead, newItems);
};

Ref<Fiber, true> Fiber::ReCreate(Ref<CodeItem, true> const&head, Array<Ref<FiberItem, true>> const& items)const{
    if(!head.IsEmpty&& !items.Where([](Ref<FiberItem, true> const&item){return item.IsEmpty; })->Any)
        return{};
    Ref<CodeItem>  newHead = this->head;
    if(!head.IsEmpty)
        newHead = head;
    Array<Ref<FiberItem>> newItems = this->items;
    for(auto index = 0; index < newItems.Count; index++)
        if(!items[index].IsEmpty)
            newItems[index] = items[index];
    return Create(newHead, newItems);
}

String const Fiber::ToCpp(CodeVisitor const&visitor) const{
    FiberVisitor localVisitor = visitor;
    auto result = head->ToCpp(localVisitor);
    for(auto item :  items) 
        result = item->ToCpp(localVisitor).Replace("$(arg)", result);
    if(size == 0)
        return result + "; return 0;";
    return "return " + result + ";";
}


pure_p_implementation(FiberConnector, Size, leftSize);
pure_p_implementation(FiberConnector, Size, rightSize);
pure_p_implementation(FiberConnector, Size, size);

String const FiberVisitor::Const(Size const size, BitsConst const& value) const {
    return value.format;
}

String const FiberVisitor::DumpPrintNumber(Size const size) const{
    return "DumpPrint($(arg))";
}

String const FiberVisitor::Pair(Ref<CodeItem> const& left, Ref<CodeItem> const& right, Ref<FiberConnector> const&connector) const {
    auto leftCode = left->ToCpp(*this);
    auto rightCode = right->ToCpp(*this);
    auto connectorCodeRaw = connector->ToCpp(*this);
    return connectorCodeRaw
        .Replace("$(left)", leftCode)
        .Replace("$(right)", rightCode);
}

String const FiberVisitor::BinaryOperation(String const& name, Size const&size, Size const&leftSize, Size const&rightSize)const {
    return "($(left)) " + name + " ($(right))";
};


