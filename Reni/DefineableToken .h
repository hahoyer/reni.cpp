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
        virtual Ref<Syntax> const CreateSyntax(Ref<Syntax >const left, SourcePart const&part, Ref<Syntax >const right)const override{
            return new ExpressionSyntax(*this, left, part, right);
        };

        virtual Ref<Syntax> const CreateSyntax(Ref<Syntax >const left, SourcePart const&part)const override{
            return new ExpressionSyntax(*this, left, part, {});
        };

        virtual Ref<Syntax> const CreateSyntax(SourcePart const&part, Ref<Syntax >const right)const override{
            return new ExpressionSyntax(*this, {}, part, right);
        };

    };



}