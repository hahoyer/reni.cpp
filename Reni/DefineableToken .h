#pragma once
#include "TokenClass.h"
#include "ExpressionSyntax.h"
#include "FeatureClass.h"

#include "../HWLib/String.h"

using namespace HWLib;

namespace Reni{
    class DefineableToken : public TokenClass{
        typedef TokenClass baseType;
        typedef DefineableToken thisType;

        DefinableFeatureClass feature;
    public:
        String const name;
    protected:
        DefineableToken(String const&name)
            : name(name)
              , feature(*this){
            feature.SetDumpString();
        };
        featureClasses_override;
    private:
        Ref<Syntax> const CreateSyntax(Ref<Syntax, true>const left, SourcePart const&part, Ref<Syntax, true>const right)const override{
            return new ExpressionSyntax(*this, left, part, right);
        };

        p_function(Array<String>, DumpData) override{
            auto result = name.Quote;
            return{result};
        };

        p_function(String, DumpShort) override{return Dump;};
    };
}