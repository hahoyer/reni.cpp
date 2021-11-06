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

namespace Reni
{
    class CopyFromAddressFiber final : public FiberItem 
    {
        typedef FiberItem baseType;
        typedef CopyFromAddressFiber  thisType;
        Type const &target;
    public:
        CopyFromAddressFiber(Type const&target)
            : target(target)
        {
            SetDumpString();
        }
    private:
        p_function(Array<string>, DumpData) override{ return{HW_D_VALUE(target)}; };
        p_function(Size, argSize) override{ return Size::Address; };
        p_function(Size, size) override{ return target.size; };
        Optional<Ref<FiberItem>> Replace(ReplaceVisitor const&) const override { return{}; }
    };
}

p_virtual_header_implementation(FiberItem, Size, argSize); 
p_virtual_header_implementation(FiberItem, Size, size); 
p_virtual_header_implementation(FiberItem, Closure, closure);

Optional<Ref<FiberItem>> FiberItem::Replace(ReplaceVisitor const& visitor) const
{
    md(visitor);
    mb;
}

Array<Ref<FiberItem>> const FiberItem::CopyFromAddress(Type const& target)
{
    a_if(target.isCopyable, HW_D_VALUE(target));
    return{new CopyFromAddressFiber(target)};
};

string FiberItem::ToCpp(CodeVisitor const& visitor) const
{
    md(visitor);
    mb;
};


FiberCode::FiberCode(Ref<CodeItem> const& head, Array<Ref<FiberItem>> const&items)
    : head(head)
    , items(items)
{
    SetDumpString();
    a_if(IsValid, Dump);
}

p_implementation(FiberCode, Size, size){ return items.Last->size; };

p_implementation(FiberCode, Closure, closure)
{
    auto itemClosure = items
        .Select<Closure>([&](Ref<FiberItem> item)
            {
                return item->closure;
            });
    return head->closure + Closure::Aggregate(itemClosure);
};


Ref<FiberCode> FiberCode::operator+(Array<Ref<FiberItem>> const&items) const
{
    return *head + (this->items + items);
}

p_implementation(FiberCode, bool, IsValid) {
    Optional<Ref<FiberItem>> lastItem;
    for(Ref<FiberItem> item: items)
    {
        Size size = lastItem.IsValid 
            ? lastItem.Value->size 
            : head->size;
        if(size != item->argSize)
            return false;
        lastItem = item;
    }
    return true;
};

Optional<Ref<CodeItem>> FiberCode::ReplaceImpl(ReplaceVisitor const& visitor) const
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

Optional<Ref<FiberCode>> FiberCode::ReCreate(Optional<Ref<CodeItem>> const&head, Array<Optional<Ref<FiberItem>>> const& items)const{
    if(!head.IsEmpty&& !items.Where([](Optional<Ref<FiberItem>> const&item){return item.IsEmpty; })->Any)
        return{};
    Ref<CodeItem>  newHead = this->head;
    if(!head.IsEmpty)
        newHead = head;
    Array<Ref<FiberItem>> newItems = this->items;
    for(auto index = 0; index < newItems.Count; index++)
        if(!items[index].IsEmpty)
            newItems[index] = items[index];
    return *newHead + newItems;
}

string FiberCode::ToCpp(CodeVisitor const& visitor) const{
    auto result = head->ToCpp(visitor);
    for(auto item :  items) 
        result = item->ToCpp(visitor)|HWLib::Replace(string("$(arg)"), result);
    return result;
}


int FiberConnectorItem::nextObjectId = 0;

p_virtual_header_implementation(FiberConnectorItem, int, inCount);
p_virtual_header_implementation(FiberConnectorItem, Size, size);
p_virtual_header_implementation(FiberConnectorItem, Closure, closure);
p_virtual_header_implementation(FiberConnectorItem, string, prefix);

