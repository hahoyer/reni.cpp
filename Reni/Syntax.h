#pragma once

#include "Result.h"

namespace Reni
{
    class Category;
    class CodeItem;
    class Context;
    class Result;
    class TokenClass;

    class Syntax : public WithId<DumpableObject, Syntax>
    {
        using baseType = WithId<DumpableObject, Syntax>;
        using thisType = Syntax;
    public:
        using TokenClass = Reni::TokenClass;

    private:
        FunctionCache<Context const*, Result > resultCache;
    protected:
        SourcePart const part;

    protected:
        Syntax(SourcePart const& part);
    public:
        virtual Ref<Syntax > const ParenthesisMatch(int level, SourcePart const&part)const {
            bool Trace = true;
            md(level, part);
            b_;
            return_d(OptRef<Syntax >());
        };

        Ref<CodeItem> Code(Context const&context)const;
        Result GetResult(Context const&context, Category category)const;
        virtual ResultData const GetResultData(Context const&context, Category category)const;
    };

    class TokenClass;

    class InfixSyntax : public Syntax
    {
        using baseType = Syntax;
        Ref<Syntax > const left;
        TokenClass const& tokenClass;
        Ref<Syntax > const right;
    public:
        InfixSyntax(Ref<Syntax > const left, TokenClass const& tokenClass, SourcePart const part, Ref<Syntax > const right)
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
        Ref<Syntax > const right;
    public:
        PrefixSyntax(TokenClass const& tokenClass, SourcePart const part, Ref<Syntax > const right)
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
        Ref<Syntax > const left;
        TokenClass const& tokenClass;
    public:
        SuffixSyntax(Ref<Syntax > const left, TokenClass const& tokenClass, SourcePart const part)
            : baseType(part)
            , left(left)
            , tokenClass(tokenClass)
        {
            SetDumpString();
        }
    private:
        override_p_function(Array<String>, DumpData);
    };


}

