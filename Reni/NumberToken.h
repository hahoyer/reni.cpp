#pragma once

#include "TokenClass.h"
#include "Terminal.h"
#include "FeatureClass.h"

namespace Reni
{
    class NumberToken final : public TerminalTokenClass{
        using baseType = TerminalTokenClass;
        using thisType = NumberToken;
        GenericFeatureClass<thisType> feature;
    public:
        ResultData const GetResultData(Context const&context, Category category, SourcePart const&part)const;
    private:
        Ref<Syntax> const Create(SourcePart const&part) const override final;
        p_function(WeakRef<FeatureClass>,featureClass) override{ return &feature.ref; }
    };
}

