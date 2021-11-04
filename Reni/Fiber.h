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
        ThisRef;
        p_virtual(Size, argSize) { return 0; };
        p_virtual(Size, size) { return 0; };
        p_virtual(Closure, closure){ return{}; };
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
        p_function(Array<string>,DumpData) override{ return{nd(head), nd(items)}; };
        p_function(Size,size) override;
        p_function(Closure, closure)override;
        Optional<Ref<FiberCode>> ReCreate(Optional<Ref<CodeItem>> const& head, Array<Optional<Ref<FiberItem>>> const& items)const;
        virtual string ToCpp(CodeVisitor const&) const override;
        p(bool, IsValid);
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
        ThisRef;
        p_virtual(int, inCount) = 0;
        p_virtual(Size, size) = 0;
        p_virtual(Closure, closure){ return{}; };
        p_virtual(string, prefix) = 0;
        virtual Size InSize(int index)const = 0;
        virtual string ToCpp(CodeVisitor const& visitor)const = 0;
        virtual Optional<Ref<FiberItem>> Replace(ReplaceVisitor const&arg) const = 0;
    };

}