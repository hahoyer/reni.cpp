#pragma once

namespace Reni
{
    class Syntax;

    class TokenClass final : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = TokenClass;
    public:
        TokenClass() = default;
        TokenClass(TokenClass const&) = delete;

        Ref<Syntax const> const CreateSyntax(Ref<Syntax const>const left, SourcePart const&part, Ref<Syntax const>const right, bool isMatch)const;
        Ref<Syntax const> const CreateSyntax(Ref<Syntax const>const left, SourcePart const&part, bool isMatch)const;
        Ref<Syntax const> const CreateSyntax(SourcePart const&part, Ref<Syntax const>const right, bool isMatch)const;
        Ref<Syntax const> const CreateSyntax(SourcePart const&part, bool isMatch)const;
    private:
        override_p_function(Array<String>, DumpData);
    };

}