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
        Ref<Syntax> const CreateSyntax(Optional<Ref<Syntax>>const left, SourcePart const&part, Optional<Ref<Syntax>>const right)const override;
    };
}

