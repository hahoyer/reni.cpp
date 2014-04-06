#pragma once

#include "TokenClass.h"
#include "Terminal.h"
#include "FeatureClass.h"
#include "../Util/Category.h"

using namespace Util;

namespace Reni
{
    class FunctionToken final : public TokenClass{
        using baseType = TokenClass;
        using thisType = FunctionToken;
        GenericFeatureClass<thisType> feature;
    public:
        p(String, name){ return "/\\"; }
    private:
        Ref<Syntax> const CreateSyntax(Ref<Syntax, true>const left, SourcePart const&part, Ref<Syntax, true>const right)const override;
        p_function(Array<WeakRef<FeatureClass>>, featureClasses) override{ return base_p_name(featureClasses) + &feature.thisRef; }
    };
}

