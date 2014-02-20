#pragma once

namespace Reni
{
    class Syntax;

    class TokenClass : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = TokenClass;
        static int nextObjectId;
    public:
        TokenClass() : baseType(nextObjectId++){};
        TokenClass(TokenClass const&) = delete;

        virtual Ref<Syntax const> const CreateSyntax(Ref<Syntax const>const left, SourcePart const&part, Ref<Syntax const>const right, bool isMatch)const = 0;
        virtual Ref<Syntax const> const CreateSyntax(Ref<Syntax const>const left, SourcePart const&part, bool isMatch)const = 0;
        virtual Ref<Syntax const> const CreateSyntax(SourcePart const&part, Ref<Syntax const>const right, bool isMatch)const = 0;
        virtual Ref<Syntax const> const CreateSyntax(SourcePart const&part, bool isMatch)const = 0;

        static Ref<TokenClass const> const Pending;
    };

    using Token = HWLang::Token<TokenClass const>const;
};