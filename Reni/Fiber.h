#pragma once
#include "Code.h"
#include "CodeVisitor.h"

using namespace HWLib;

namespace Reni{
    class CodeItem;
    class FiberItem;
    class Size;
    class ReplaceVisitor;

    class FiberHead
        : public CodeItem{
        typedef CodeItem baseType;
        typedef FiberHead thisType;
    public:
        ref_p;
        Ref<CodeItem> const Fiber(Array<Ref<FiberItem>> const&items)const;
    };


    class FiberItem
        : public DumpableObject
          , public RefCountProvider{
        typedef DumpableObject baseType;
        typedef CodeItem thisType;
    public:
        virtual_p(Size, inSize) = 0;
        virtual_p(Size, outSize) = 0;
        virtual String const ToCpp(CodeVisitor const& visitor)const;
    };


    class Fiber final: public CodeItem{
        typedef CodeItem baseType;
        typedef Fiber thisType;
        Ref<FiberHead> const head;
        Array<Ref<FiberItem>> const items;

        Fiber(Ref<FiberHead> const& head, Array<Ref<FiberItem>> const& items)
            : head(head)
              , items(items){
            SetDumpString();
        }
    public:
        static Ref<Fiber> Create(Ref<FiberHead> const& head, Array<Ref<FiberItem>> const& items){
            return new thisType(head, items);
        }
    private:
        virtual Ref<CodeItem> const Replace(ReplaceVisitor const&arg) const override;
        override_p_function(Array<String>, DumpData){ return{nd(head), nd(items)}; };
        override_p_function(Size, size);
    };
}