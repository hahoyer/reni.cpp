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
    private:
        GenericFeatureClass<thisType> feature;
        p_function(WeakRef<FeatureClass>, featureClass) override {
            return &feature.ref;
        }
    };


}

