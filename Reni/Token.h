#pragma once

namespace Reni
{
    class Syntax;
    class TokenClass;
    class Token final: public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = Token;
    public:
        TokenClass const&Class;
        SourcePart const Part;

        Token(TokenClass const&class_, SourcePart const& part)
            : Class(class_)
            , Part(part)
        {
            SetDumpString();
        };

        DefaultAssignmentOperator;

        p(String, Name){ return Part; };
        p(bool, IsEnd){ return Part.IsEnd; };

        Ref<Syntax const> const Create(OptRef<Syntax const> const left, OptRef<Syntax const> right)const;

    private:
        virtual p_function(Array<String>, DumpData)override;
        virtual p_function(String, DumpShort)override;
    };

}
