#pragma once
#include "TokenClass.h"

namespace Reni{
    class SyntaxErrorToken final : public TokenClass{
        using baseType = TokenClass;
        using thisType = SyntaxErrorToken;
        String const text;
    public:
        SyntaxErrorToken(String const text) : text(text){}
    private:
        p_function(Array<String>,DumpData) override{
            return{nd(text)};
        };

        GenericFeatureClass<thisType> feature;
        p_function(WeakRef<FeatureClass>,featureClass) override{ return &feature.ref; }
    };
}