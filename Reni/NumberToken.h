#pragma once

#include "TokenClass.h"
#include "Terminal.h"

namespace Reni
{
    class NumberToken final : public TokenClass, public TerminalTokenClass{
        using baseType = TokenClass;
        using thisType = NumberToken;

        virtual Ref<Syntax> const CreateSyntax(SourcePart const&part, bool isMatch)const override;
        virtual ResultData const GetResultData(Context const&context, Category category, SourcePart const&part)const override;
    };
}

