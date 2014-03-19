#pragma once
#include "TokenClass.h"
#include "../HWLib/String.h"
#include "ExpressionSyntax.h"
using namespace HWLib;

namespace Reni{
    class DefineableToken : public TokenClass{
        typedef TokenClass baseType;
        typedef DefineableToken thisType;
    public:
        String const name;
    protected:
        DefineableToken(String const&name) : name(name){};
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