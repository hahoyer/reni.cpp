#pragma once

namespace Reni
{
    class Syntax;
    class TokenClass;
    struct Token final: public DumpableObject
    {
        TokenClass const&Class;
        SourcePart const Part;

        Token(TokenClass const&class_, SourcePart const& part)
            : Class(class_)
            , Part(part)
        {}

        p(String, Name){ return Part; };
        p(bool, IsEnd){ return Part.IsEnd; };

        Ref<Syntax const> const Create(OptRef<Syntax const> const left, OptRef<Syntax const> right)const;

    private:
        p_function(String, Dump)override;
    };

}
