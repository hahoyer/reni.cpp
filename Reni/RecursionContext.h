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
        ThisRef;

        Optional<WeakRef<Type>> const CachedType(Syntax const& target) const;
    private:
        p_function(WeakRef<FunctionCallContext>, functionContext) override{ return parent.functionContext; };
        p_function(WeakRef<Global>, global) override{ return parent.global; };
        p_function(Array<string>,DumpData) override{ return{HW_D_VALUE(parent)}; }
        p_function(bool, isRecursion) override{ return true; };
        p_function(WeakRef<RecursionContext>, recursionContext) override;

        WeakRef<Type> const FunctionType(FunctionSyntax const& body) const override;
        WeakRef<ContainerContext> const Container(SyntaxContainer const& syntax, size_t index) const override;
        SearchResult<AccessFeature> const DeclarationsForType(DefineableToken const& token) const override;
    };

}


