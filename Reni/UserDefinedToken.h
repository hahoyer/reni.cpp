#pragma once

#include "TokenClass.h"
#include "Terminal.h"
#include "FeatureClass.h"
#include "../Util/Category.h"
#include "DefineableToken .h"

using namespace Util;

namespace Reni {
    class UserDefinedToken final : public DefineableToken {
        using baseType = DefineableToken;
        using thisType = UserDefinedToken;
    public:
        UserDefinedToken(String const&name) : baseType(name) {
            SetDumpString();
        };
    };
}
