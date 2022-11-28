#pragma once
#include "TokenClass.h"

namespace Reni{
    class SyntaxErrorToken final : public TerminalTokenClass{
        using baseType = TokenClass;
        using thisType = SyntaxErrorToken;
        String const text;
    public:
        SyntaxErrorToken(String const text) : text(text){}
    private:
        Ref<Syntax> const Create(SourcePart const&part)const override final;
        HW_PR_DECL_GETTER(Array<string>,DumpData) override{
            return{HW_D_VALUE(text)};
        };
    };
}