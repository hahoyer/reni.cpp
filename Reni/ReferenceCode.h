#pragma once
#include "CodeItem.h"

namespace Reni
{
    class ContainerContext;

    class ReferenceCode final : public CodeItem
    {
        typedef CodeItem baseType;
        typedef ReferenceCode thisType;
        ContainerContext const& value;
    public:
        explicit ReferenceCode(ContainerContext const& value);
    private:
        HW_PR_DECL_GETTER(Array<string>, DumpData) override;
        HW_PR_DECL_GETTER(Size, size) override{return Size::Address;}
        HW_PR_DECL_GETTER(Closure, closure) override;

        Ref<CodeItem> ReferencePlus(Size offset) const override;
        Optional<Ref<CodeItem>> ReplaceImpl(ReplaceVisitor const& arg) const override;
    };
}