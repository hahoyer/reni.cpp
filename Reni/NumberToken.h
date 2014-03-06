#pragma once

#include "TokenClass.h"
#include "Terminal.h"
#include "FeatureClass.h"

namespace Reni
{
    class NumberToken final : public TokenClass, public TerminalTokenClass{
        using baseType = TokenClass;
        using thisType = NumberToken;

        virtual CtrlRef<Syntax> const CreateSyntax(SourcePart const&part, bool isMatch)const override;
        virtual ResultData const GetResultData(Context const&context, Category category, SourcePart const&part)const override;
    private:
        GenericFeatureClass<thisType> feature;
        override_p_function(WeakRef<FeatureClass>, featureClass){ return &feature.ref; }
    };
}

