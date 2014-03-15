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
            a_if_(IsValid);
        }
    public:
        static Ref<Fiber> Create(Ref<CodeItem> const& head, Array<Ref<FiberItem>> const& items);
    private:
        virtual Ref<CodeItem,true> const Replace(ReplaceVisitor const&arg) const override;
        p_function(Array<String>,DumpData) override{ return{nd(head), nd(items)}; };
        p_function(Size,size) override;
        Ref<Fiber, true> ReCreate(Ref<CodeItem, true> const& head, Array<Ref<FiberItem, true>> const& items)const;
        virtual String const ToCpp(CodeVisitor const&) const override;
        p(bool, IsValid);
    };


    class FiberVisitor final : public CodeVisitor{
        typedef CodeVisitor baseType;
        typedef FiberVisitor thisType;
        CodeVisitor const&parent;
    public: 
        FiberVisitor(CodeVisitor const&parent) : parent(parent) {}
    private:
        p_function(Array<String>,DumpData) override {return{ nd(parent) };};
        virtual String const Const(Size const size, BitsConst const& value) const override;
        virtual String const DumpPrintNumber(Size const size) const override;
    };
}