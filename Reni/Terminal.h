#pragma once

#include "Syntax.h"

namespace Reni
{
    class Context;
    class Category;
    class ResultData;

    class TerminalTokenClass{
    public:
        virtual ResultData const GetResultData(Context const&context, Category category, SourcePart const&part)const;
    };


    class TerminalSyntax : public Syntax
    {
        using baseType = Syntax;
        TerminalTokenClass const& tokenClass;
    public:
        TerminalSyntax(TerminalTokenClass const& tokenClass, SourcePart const part)
            : baseType(part)
            , tokenClass(tokenClass)
        {
            SetDumpString();
        }
    private:
        override_p_function(Array<String>, DumpData);
        virtual ResultData const GetResultData(Context const&context, Category category)const override;
    };
};