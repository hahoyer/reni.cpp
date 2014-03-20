#pragma once

#include "Syntax.h"

namespace Reni
{
    class Context;
    class Category;
    class ResultData;

    template <class TTokenClass>
    class TerminalSyntax : public Syntax
    {
        typedef Syntax baseType;
        typedef TerminalSyntax thisType;
        TTokenClass const& tokenClass;
    public:
        TerminalSyntax(TTokenClass const& tokenClass, SourcePart const part)
            : baseType(part)
            , tokenClass(tokenClass)
        {
            SetDumpString();
        }
    private:
        p_function(Array<String>,DumpData) override{ return{nd(tokenClass)}; };

        ResultData const GetResultData(Context const&context, Category category)const override{
            return tokenClass.GetResultData(context, category, part);
        }

        Ref<Syntax, true> Replace(SyntaxArgVisitor const&visitor) const override{return tokenClass.Replace(visitor);};
    };
};