#pragma once
#include "TokenClass.h"
#include "../HWLib/String.h"
#include "ExpressionSyntax.h"
using namespace HWLib;

namespace Reni{
    class DefineableToken : public TokenClass{
        using baseType = TokenClass;
        using thisType = DefineableToken;
    public:
        String const name;
    protected:
        DefineableToken(String const&name) : name(name){};
    private:
        virtual Ref<Syntax> const CreateSyntax(Ref<Syntax >const left, SourcePart const&part, bool isMatch)const override{
            if (isMatch)
                return baseType::CreateSyntax(left, part, isMatch);
            return new ExpressionSyntax(*this, left, part, {});
        };

        virtual Ref<Syntax> const CreateSyntax(SourcePart const&part, Ref<Syntax >const right, bool isMatch)const override{
            if (isMatch)
                return baseType::CreateSyntax(part, right, isMatch);
            return new ExpressionSyntax(*this, {}, part, right);
        };

    };



}