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

        virtual Ref<Syntax> const CreateSyntax(Ref<Syntax>const left, SourcePart const&part, Ref<Syntax>const right, bool isMatch)const = 0;
        virtual Ref<Syntax> const CreateSyntax(Ref<Syntax>const left, SourcePart const&part, bool isMatch)const = 0;
        virtual Ref<Syntax> const CreateSyntax(SourcePart const&part, Ref<Syntax>const right, bool isMatch)const = 0;
        virtual Ref<Syntax> const CreateSyntax(SourcePart const&part, bool isMatch)const = 0;

        static Ref<TokenClass> const Pending;
    };

    using Token = HWLang::Token<TokenClass>;
};