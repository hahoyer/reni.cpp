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
        override_p_function(Optional<bool>, HasLeft){ return false; }
        override_p_function(Optional<bool>, HasRight){ return false; }
        Ref<Syntax> const Terminal(SourcePart const&part) const override;
        override_p_function(WeakRef<FeatureClass>, featureClass){ return &feature.ref; }
    };
}

