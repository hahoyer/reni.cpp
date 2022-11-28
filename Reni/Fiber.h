#pragma once
#include "CodeItem.h"
#include "CodeVisitor.h"
#include "Closure.h"

using namespace HWLib;
using namespace Util;

namespace Reni{
    class CodeItem;
    class FiberItem;
    class ReplaceVisitor;

    class FiberItem
        : public DumpableObject
          , public RefCountProvider{
        typedef DumpableObject baseType;
        typedef FiberItem thisType;
    public:
        HW_PR_THISREF;
        HW_PR_VIRTUAL_GET(Size, argSize) { return 0; };
        HW_PR_VIRTUAL_GET(Size, size) { return 0; };
        HW_PR_VIRTUAL_GET(Closure, closure){ return{}; };
        virtual string ToCpp(CodeVisitor const& visitor) const;
        virtual Optional<Ref<FiberItem>> Replace(ReplaceVisitor const& arg) const;
        static Array<Ref<FiberItem>> const CopyFromAddress(Type const& target);
    };


    class FiberCode final: public CodeItem{
        typedef CodeItem baseType;
        typedef FiberCode thisType;
        Ref<CodeItem> const head;
        Array<Ref<FiberItem>> const items;
    public:
        FiberCode(Ref<CodeItem> const& head, Array<Ref<FiberItem>> const&items);
    private:
        virtual Optional<Ref<CodeItem>> ReplaceImpl(ReplaceVisitor const& arg) const override;
        Ref<FiberCode> operator+(Array<Ref<FiberItem>> const&items) const override;
        HW_PR_DECL_GETTER(Array<string>,DumpData) override{ return{HW_D_VALUE(head), HW_D_VALUE(items)}; };
        HW_PR_DECL_GETTER(Size,size) override;
        HW_PR_DECL_GETTER(Closure, closure)override;
        Optional<Ref<FiberCode>> ReCreate(Optional<Ref<CodeItem>> const& head, Array<Optional<Ref<FiberItem>>> const& items)const;
        virtual string ToCpp(CodeVisitor const&) const override;
        HW_PR_GET(bool, IsValid);
    };

    class FiberConnectorItem 
        : public DumpableObject
        , public RefCountProvider {
        typedef DumpableObject baseType; 
        typedef FiberConnectorItem thisType;
        static int nextObjectId;
        int const objectId;

    protected:
        FiberConnectorItem()
            : objectId(nextObjectId++) {}

    public:
        HW_PR_THISREF;
        HW_PR_VIRTUAL_GET(int, inCount) = 0;
        HW_PR_VIRTUAL_GET(Size, size) = 0;
        HW_PR_VIRTUAL_GET(Closure, closure){ return{}; };
        HW_PR_VIRTUAL_GET(string, prefix) = 0;
        virtual Size InSize(int index)const = 0;
        virtual string ToCpp(CodeVisitor const& visitor)const = 0;
        virtual Optional<Ref<FiberItem>> Replace(ReplaceVisitor const&arg) const = 0;
    };

}