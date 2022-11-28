#pragma once
#include "Context.h"

namespace Reni
{
    class RecursionContext final : public Context
    {
        using baseType = Context;
        using thisType = RecursionContext;

        RegularContext const&parent;
    public:
        explicit RecursionContext(RegularContext const&parent) : parent(parent)
        {
            SetDumpString();
        }
        HW_PR_THISREF;

        Optional<WeakRef<Type>> const CachedType(Syntax const& target) const;
    private:
        HW_PR_DECL_GETTER(WeakRef<FunctionCallContext>, functionContext) override{ return parent.functionContext; };
        HW_PR_DECL_GETTER(WeakRef<Global>, global) override{ return parent.global; };
        HW_PR_DECL_GETTER(Array<string>,DumpData) override{ return{HW_D_VALUE(parent)}; }
        HW_PR_DECL_GETTER(bool, isRecursion) override{ return true; };
        HW_PR_DECL_GETTER(WeakRef<RecursionContext>, recursionContext) override;

        WeakRef<Type> const FunctionType(FunctionSyntax const& body) const override;
        WeakRef<ContainerContext> const Container(SyntaxContainer const& syntax, size_t index) const override;
        SearchResult<AccessFeature> const DeclarationsForType(DefineableToken const& token) const override;
    };

}


