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

        WeakPtr<Type> const CachedType(Syntax const& target) const ;
    private:
        p_function(WeakRef<FunctionCallContext>, functionContext) override{ return parent.functionContext; };
        p_function(WeakRef<Global>, global) override{ return parent.global; };
        p_function(Array<String>, DumpData) override{ return{nd(parent)}; }
        p_function(bool, isRecursion) override{ return true; };

        WeakRef<Type> const FunctionType(FunctionSyntax const& body) const override;
        WeakRef<Context> const Container(SyntaxContainer const& syntax, int index) const override;
        SearchResult<ContextFeature> const Search(DefineableToken const& token) const override;
    };

}


