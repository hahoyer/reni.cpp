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
        Ref<Syntax> const CreateSyntax(SourcePart const&part)const override;
        override_p_function(WeakRef<FeatureClass>, featureClass){ return &feature.ref; }
    };
}

