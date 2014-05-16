#pragma once

#include "TokenClass.h"
#include "Terminal.h"
#include "../Util/Category.h"

using namespace Util;

namespace Reni
{
    class NumberToken final : public TerminalTokenClass{
        using baseType = TerminalTokenClass;
        using thisType = NumberToken;
    public:
        ResultData const GetResultData(Context const&context, Category category, SourcePart const&part)const;
    private:
        Ref<Syntax> const Create(SourcePart const&part) const override final;
    };
}

