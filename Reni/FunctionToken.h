#pragma once

#include "TokenClass.h"
#include "Terminal.h"
#include "../Util/Category.h"

using namespace Util;

namespace Reni
{
    class FunctionToken final : public TokenClass{
        using baseType = TokenClass;
        using thisType = FunctionToken;
    public:
        p(String, name){ return "/\\"; }
    private:
        Ref<Syntax> const CreateSyntax(Ref<Syntax, true>const left, SourcePart const&part, Ref<Syntax, true>const right)const override;
    };
}

