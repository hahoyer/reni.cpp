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
        ResultData const GetResult(Category category, Syntax const& syntax) const;
    private:
        p_function(WeakRef<FunctionCallContext>, functionContext) override{ return parent.functionContext; };
        p_function(WeakRef<Global>, global) override{ return parent.global; };
        p_function(Array<String>, DumpData) override{ return{nd(parent)}; }
        p_function(bool, isRecursion) override{ return true; };
        p_function(WeakRef<RecursionContext>, recursionContext) override;

        WeakRef<Type> const FunctionType(FunctionSyntax const& body) const override;
        WeakRef<ContainerContext> const Container(SyntaxContainer const& syntax, int index) const override;
        SearchResult<AccessFeature> const DeclarationsForType(DefineableToken const& token) const override;
    };

}


