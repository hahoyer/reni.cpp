#pragma once

namespace Reni
{
    class Syntax : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = Syntax;
    protected:
        Syntax();
    };

    class TokenClass;

    class InfixSyntax : public Syntax
    {
        Ref<Syntax const> const left;
        TokenClass const& tokenClass;
        Ref<Syntax const> const right;
    public:
        InfixSyntax(Ref<Syntax const> const left, TokenClass const& tokenClass, Ref<Syntax const> const right)
            : left(left)
            , tokenClass(tokenClass)
            , right(right)
        {
            SetDumpString();
        }
    private:
        virtual p_function(Array<String>, DumpData)override;
    };

    class PrefixSyntax : public Syntax
    {
        TokenClass const& tokenClass;
        Ref<Syntax const> const right;
    public:
        PrefixSyntax(TokenClass const& tokenClass, Ref<Syntax const> const right)
            : tokenClass(tokenClass)
            , right(right)
        {
            SetDumpString();
        }
    private:
        virtual p_function(Array<String>, DumpData)override;
    };

    class SuffixSyntax : public Syntax
    {
        Ref<Syntax const> const left;
        TokenClass const& tokenClass;
    public:
        SuffixSyntax(Ref<Syntax const> const left, TokenClass const& tokenClass)
            : left(left)
            , tokenClass(tokenClass)
        {
            SetDumpString();
        }
    private:
        virtual p_function(Array<String>, DumpData)override;
    };

    class TerminalSyntax : public Syntax
    {
        TokenClass const& tokenClass;
        SourcePart const part;
    public:
        TerminalSyntax(TokenClass const& tokenClass, SourcePart part)
            : tokenClass(tokenClass)
            , part(part)
        {
            SetDumpString(); 
        }
    private:
        virtual p_function(Array<String>, DumpData)override;
    };

}

DumpFromRef(Reni::Syntax);
DumpFromRef(Reni::InfixSyntax);
DumpFromRef(Reni::PrefixSyntax);
DumpFromRef(Reni::SuffixSyntax);
DumpFromRef(Reni::TerminalSyntax);

DetailedDump(Reni::Syntax);
DetailedDump(Reni::InfixSyntax);
DetailedDump(Reni::PrefixSyntax);
DetailedDump(Reni::SuffixSyntax);
DetailedDump(Reni::TerminalSyntax);

