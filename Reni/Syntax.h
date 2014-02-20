#pragma once

namespace Reni
{
    class Syntax : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = Syntax;
    public:
        using TokenClass = Reni::TokenClass;
    protected:
        SourcePart const part;
        Syntax(SourcePart const& part);
    public:
        virtual Ref<Syntax const> const ParenthesisMatch(int level, SourcePart const&part)const {
            bool Trace = true;
            md(level, part);
            b_;
            return_d(OptRef<Syntax const>());

        };
    };

    class TokenClass;

    class InfixSyntax : public Syntax
    {
        using baseType = Syntax;
        Ref<Syntax const> const left;
        TokenClass const& tokenClass;
        Ref<Syntax const> const right;
    public:
        InfixSyntax(Ref<Syntax const> const left, TokenClass const& tokenClass, SourcePart const part, Ref<Syntax const> const right)
            : baseType(part)
            , left(left)
            , tokenClass(tokenClass)
            , right(right)
        {
            SetDumpString();
        }
    private:
        override_p_function(Array<String>, DumpData);
    };

    class PrefixSyntax : public Syntax
    {
        using baseType = Syntax;
        TokenClass const& tokenClass;
        Ref<Syntax const> const right;
    public:
        PrefixSyntax(TokenClass const& tokenClass, SourcePart const part, Ref<Syntax const> const right)
            : baseType(part)
            , tokenClass(tokenClass)
            , right(right)
        {
            SetDumpString();
        }
    private:
        override_p_function(Array<String>, DumpData);
    };

    class SuffixSyntax : public Syntax
    {
        using baseType = Syntax;
        Ref<Syntax const> const left;
        TokenClass const& tokenClass;
    public:
        SuffixSyntax(Ref<Syntax const> const left, TokenClass const& tokenClass, SourcePart const part)
            : baseType(part)
            , left(left)
            , tokenClass(tokenClass)
        {
            SetDumpString();
        }
    private:
        override_p_function(Array<String>, DumpData);
    };

    class TerminalSyntax : public Syntax
    {
        using baseType = Syntax;
        TokenClass const& tokenClass;
    public:
        TerminalSyntax(TokenClass const& tokenClass, SourcePart const part)
            : baseType(part)
            , tokenClass(tokenClass)
        {
            SetDumpString(); 
        }
    private:
        override_p_function(Array<String>, DumpData);
    };

}

