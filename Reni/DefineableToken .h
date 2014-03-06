#pragma once
#include "TokenClass.h"
#include "../HWLib/String.h"
#include "ExpressionSyntax.h"
using namespace HWLib;

namespace Reni{
    class DefineableToken : public TokenClass{
        using baseType = TokenClass;
        using thisType = DefineableToken;

        String const name;
    protected:
        DefineableToken(String const name) : name(name){}
    private:
        override_p_function(Array<String>, DumpData){
            return{ nd(name) };
        };

        virtual CtrlRef<Syntax> const CreateSyntax(CtrlRef<Syntax >const left, SourcePart const&part, bool isMatch)const override{
            if (isMatch)
                return baseType::CreateSyntax(left, part, isMatch);
            return new ExpressionSyntax(*this, left, part, {});
        };

        virtual CtrlRef<Syntax> const CreateSyntax(SourcePart const&part, CtrlRef<Syntax >const right, bool isMatch)const override{
            if (isMatch)
                return baseType::CreateSyntax(part, right, isMatch);
            return new ExpressionSyntax(*this, {}, part, right);
        };

    };



}