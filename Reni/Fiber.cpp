#include "Import.h"
#include "Fiber.h"

#include "../HWLib/RefCountContainer.instance.h"
#include "../HWLib/DumpToString.h"
#include "../Util/BitsConst.h"
#include "Result.h"
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


Optional<Ref<FiberItem>> const FiberItem::Replace(ReplaceVisitor const&visitor) const {
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

p_implementation(Fiber, Externals, exts){
    md_;
    mb;
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

Optional<Ref<CodeItem>> const Fiber::ReplaceImpl(ReplaceVisitor const&visitor) const
{
    auto newHead = head->Replace(visitor);
    Array<Optional<Ref<FiberItem>>> newItems = items
        .Select<Optional<Ref<FiberItem>>>([&](Ref<FiberItem> item)
            {
                return item->Replace(visitor);
            })
        ->ToArray;
    auto result = ReCreate(newHead, newItems);
    if(result.IsValid )
        return Ref<CodeItem>(result.Value->thisRef);
    return{};
};

Optional<Ref<Fiber>> Fiber::ReCreate(Optional<Ref<CodeItem>> const&head, Array<Optional<Ref<FiberItem>>> const& items)const{
    if(!head.IsEmpty&& !items.Where([](Optional<Ref<FiberItem>> const&item){return item.IsEmpty; })->Any)
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


int FiberConnectorItem::nextObjectId = 0;

pure_p_implementation(FiberConnectorItem, int, inCount);
pure_p_implementation(FiberConnectorItem, Size, size);
pure_p_implementation(FiberConnectorItem, Externals, exts);

String const FiberConnectorItem::InName(int index) const
{
    return
        "$("
        + DumpTypeName(*this)
        + "."
        + String::Convert(objectId)
        + "."
        + String::Convert(index)
        + ")";

}

String const FiberVisitor::Const(Size const size, BitsConst const& value) const
{
    return value.format;
}

String const FiberVisitor::DumpPrintNumber(Size const size) const
{
    return "DumpPrint($(arg))";
}


String const FiberVisitor::FiberConnection(Array<Ref<CodeItem>> const& items, Ref<FiberConnectorItem> const&connector) const
{
    auto connectorCodeRaw = connector->ToCpp(*this);
    auto index = 0;
    return items
        .Aggregate<String>
        (
            connectorCodeRaw,
            [&]
            (String const &current, Ref<CodeItem> const &item)
            {
                return current.Replace(connector->InName(index++), item->ToCpp(*this));
            }
        );
}

String UnrefCode(int depth, String const&target)
{
    if(depth == 0)
        return "("+target+")";
    if(depth == 1)
        return "(*reinterpret_cast<int const*>" + target+ ")";
    fd(depth, target);
    b_;
    return target;
}


String const FiberVisitor::BinaryOperation(String const& name, Size const&, int leftDepth, Size const&, int rightDepth, Size const&)const
{
    return UnrefCode(leftDepth, "$(left)") + " " + name + " " + UnrefCode(rightDepth, "$(right)");
};


