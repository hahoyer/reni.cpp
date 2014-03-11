#pragma once
#include "Code.h"
#include "CodeVisitor.h"

using namespace HWLib;

namespace Reni{
    class CodeItem;
    class FiberItem;
    class Size;
    class ReplaceVisitor;

    class FiberItem
        : public DumpableObject
          , public RefCountProvider{
        typedef DumpableObject baseType;
        typedef CodeItem thisType;
    public:
        virtual_p(Size, inSize) = 0;
        virtual_p(Size, outSize) = 0;
        virtual String const ToCpp(CodeVisitor const& visitor)const;
        virtual Ref<FiberItem,true> const Replace(ReplaceVisitor const&arg) const;
    };


    class Fiber final: public CodeItem{
        typedef CodeItem baseType;
        typedef Fiber thisType;
        Ref<CodeItem> const head;
        Array<Ref<FiberItem>> const items;

        Fiber(Ref<CodeItem> const& head, Array<Ref<FiberItem>> const& items)
            : head(head)
              , items(items){
            SetDumpString();
        }
    public:
        static Ref<Fiber> Create(Ref<CodeItem> const& head, Array<Ref<FiberItem>> const& items) {
            return new thisType(head, items);
        }
    private:
        virtual Ref<CodeItem,true> const Replace(ReplaceVisitor const&arg) const override;
        override_p_function(Array<String>, DumpData){ return{nd(head), nd(items)}; };
        override_p_function(Size, size);
        Ref<Fiber, true> ReCreate(Ref<CodeItem, true> const& head, Array<Ref<FiberItem, true>> const& items)const;
    };
}