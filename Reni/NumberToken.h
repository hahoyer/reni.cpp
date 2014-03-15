#pragma once

#include "TokenClass.h"
#include "Terminal.h"
#include "FeatureClass.h"

namespace Reni
{
    class NumberToken final : public TokenClass{
        using baseType = TokenClass;
        using thisType = NumberToken;
        GenericFeatureClass<thisType> feature;
    public:
        ResultData const GetResultData(Context const&context, Category category, SourcePart const&part)const;
    private:
        p_function(Optional<bool>,HasLeft) override{ return false; }
        p_function(Optional<bool>,HasRight) override{ return false; }
        Ref<Syntax> const Terminal(SourcePart const&part) const override;
        p_function(WeakRef<FeatureClass>,featureClass) override{ return &feature.ref; }
    };
}

