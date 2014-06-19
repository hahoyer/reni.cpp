#pragma once
#include "CodeItem.h"
#include "CodeVisitor.h"
#include "Externals.h"

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
        typedef CodeItem thisType;
    public:
        virtual_p(Size, argSize) { return 0; };
        virtual_p(Size, size) { return 0; };
        virtual_p(Externals, exts){ return{}; };
        virtual String const ToCpp(CodeVisitor const& visitor)const;
        virtual Optional<Ref<FiberItem>> const Replace(ReplaceVisitor const&arg) const;
    };


    class FiberCode final: public CodeItem{
        typedef CodeItem baseType;
        typedef FiberCode thisType;
        Ref<CodeItem> const head;
        Array<Ref<FiberItem>> const items;
    public:
        FiberCode(Ref<CodeItem> const& head, Array<Ref<FiberItem>> const& items);
    private:
        virtual Optional<Ref<CodeItem>> const ReplaceImpl(ReplaceVisitor const&arg) const override;
        Ref<FiberCode> const Fiber(Array<Ref<FiberItem>> const& items) const override;
        p_function(Array<String>,DumpData) override{ return{nd(head), nd(items)}; };
        p_function(Size,size) override;
        p_function(Externals, exts)override;
        Optional<Ref<FiberCode>> ReCreate(Optional<Ref<CodeItem>> const& head, Array<Optional<Ref<FiberItem>>> const& items)const;
        virtual String const ToCpp(CodeVisitor const&) const override;
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
        virtual_p(int, inCount) = 0;
        virtual_p(Size, size) = 0;
        virtual_p(Externals, exts){ return{}; };
        virtual Size const InSize(int index)const = 0;
        virtual String const InName(int index)const;
        virtual String const ToCpp(CodeVisitor const& visitor)const = 0;
        virtual Optional<Ref<FiberItem>> const Replace(ReplaceVisitor const&arg) const = 0;
    };

    class FiberVisitor final : public CodeVisitor{
        typedef CodeVisitor baseType;
        typedef FiberVisitor thisType;
        CodeVisitor const&parent;
    public: 
        FiberVisitor(CodeVisitor const&parent) : parent(parent) {}
    private:
        p_function(Array<String>,DumpData) override {return{ nd(parent) };};
        String const Const(Size const size, BitsConst const& value) const override;
        String const DumpPrintNumber(Size const size) const override;
        String const FiberConnection(Array<Ref<CodeItem>> const&items, Ref<FiberConnectorItem> const&connector) const override;
        String const BinaryOperation(String const& name, Size const&size, int leftDepth, Size const&leftSize, int rightDepth, Size const&rightSize)const override;
        String const CallGetter(Size const& result, int const index, Size const& args) const override;
    };
}