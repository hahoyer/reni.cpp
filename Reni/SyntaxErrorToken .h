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
        override_p_function(Array<String>, DumpData){
            return{nd(text)};
        };

        GenericFeatureClass<thisType> feature;
        override_p_function(WeakRef<FeatureClass>, featureClass){ return &feature.ref; }
    };
}