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
        p_function(Array<String>, DumpData) override;
        p_function(Size, size) override{return Size::Address;}
        p_function(Externals, exts) override;

        Ref<CodeItem> const ReferencePlus(Size offset) const override;
        Optional<Ref<CodeItem>> const ReplaceImpl(ReplaceVisitor const& arg) const override;
    };
}