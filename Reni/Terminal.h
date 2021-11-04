#pragma once

#include "Syntax.h"

namespace Reni
{
    class Context;
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
        p_function(string, SmartDump) override{ return part; };
        p_function(int, priority) override{ return -2; }

        ResultData GetResultData(const Context& context, Category const& category)const override{
            return tokenClass.GetResultData(context, category, part);
        }

        Optional<Ref<Syntax>> Replace(SyntaxArgVisitor const&visitor) const override{ return tokenClass.Replace(visitor); };
    };
};