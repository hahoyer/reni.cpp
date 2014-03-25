#pragma once
#include "Syntax.h"
using namespace HWLib;

#include "Context.h"
#include "SearchResult.h"

namespace Reni
{
    class ConditionalSyntax : public Syntax
    {
        typedef Syntax baseType;
        typedef ConditionalSyntax thisType;
    public:
        Ref<Syntax> const left;
        Ref<Syntax> const right;
        ConditionalSyntax (Ref<Syntax> const left, SourcePart const part, Ref<Syntax> const right);
    private:
        p_function(Array<String>, DumpData) override;
        virtual ResultData const GetResultData(Context const&context, Category category)const override;
        virtual Ref<Syntax, true> Replace(SyntaxArgVisitor const&) const override;
    };
}