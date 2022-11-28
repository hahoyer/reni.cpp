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
        HW_PR_DECL_GETTER(Array<string>, DumpData) override{ return{HW_D_VALUE(target)}; };
        HW_PR_DECL_GETTER(Size, argSize) override{ return Size::Address; };
        HW_PR_DECL_GETTER(Size, size) override{ return target.size; };
        Optional<Ref<FiberItem>> Replace(ReplaceVisitor const&) const override { return{}; }
    };
}

HW_PR_VIRTUAL_GETTER_WRAPPER(FiberItem, Size, argSize); 
HW_PR_VIRTUAL_GETTER_WRAPPER(FiberItem, Size, size); 
HW_PR_VIRTUAL_GETTER_WRAPPER(FiberItem, Closure, closure);

Optional<Ref<FiberItem>> FiberItem::Replace(ReplaceVisitor const& visitor) const
{
    HW_D_METHOD(visitor);
    HW_BREAK_AND_THROW;
}

Array<Ref<FiberItem>> const FiberItem::CopyFromAddress(Type const& target)
{
    HW_ASSERT(target.isCopyable, HW_D_VALUE(target));
    return{new CopyFromAddressFiber(target)};
};

string FiberItem::ToCpp(CodeVisitor const& visitor) const
{
    HW_D_METHOD(visitor);
    HW_BREAK_AND_THROW;
};


FiberCode::FiberCode(Ref<CodeItem> const& head, Array<Ref<FiberItem>> const&items)
    : head(head)
    , items(items)
{
    SetDumpString();
    HW_ASSERT(IsValid, Dump);
}

HW_PR_IMPL_GETTER(FiberCode, Size, size){ return items.Last->size; };

HW_PR_IMPL_GETTER(FiberCode, Closure, closure)
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

HW_PR_IMPL_GETTER(FiberCode, bool, IsValid) {
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

HW_PR_VIRTUAL_GETTER_WRAPPER(FiberConnectorItem, int, inCount);
HW_PR_VIRTUAL_GETTER_WRAPPER(FiberConnectorItem, Size, size);
HW_PR_VIRTUAL_GETTER_WRAPPER(FiberConnectorItem, Closure, closure);
HW_PR_VIRTUAL_GETTER_WRAPPER(FiberConnectorItem, string, prefix);

