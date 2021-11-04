#pragma once

#include "TokenClass.h"
#include "Terminal.h"
#include "../Util/Category.h"
#include "DefineableToken.h"

using namespace Util;

namespace Reni {
    class UserDefinedToken final : public DefineableToken {
        using baseType = DefineableToken;
        using thisType = UserDefinedToken;
    public:
        UserDefinedToken(string const&name) : baseType(name) {
            SetDumpString();
        };
    };
}
