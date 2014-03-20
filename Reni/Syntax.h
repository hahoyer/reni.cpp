#pragma once

#include "Result.h"
#include "..\HWLang\SourcePart.h"
#include "..\HWLib\FunctionCache.h"
using namespace HWLib;
using namespace HWLang;

namespace Reni
{
    class CodeItem;
    class Context;
    class ResultFromSyntaxAndContext;
    class ResultCache;
    class TokenClass;
    class Type;
    class SyntaxArgVisitor;

    class Syntax 
        : public WithId<DumpableObject, Syntax>
        , public RefCountProvider
    {
        using baseType = WithId<DumpableObject, Syntax>;
        using thisType = Syntax;
    public:
        using TokenClass = Reni::TokenClass;

    private:
        FunctionCache<Context const*, Ref<ResultFromSyntaxAndContext>> resultCache;
    protected:
        SourcePart const part;
        Syntax(SourcePart const& part);
    public:
        virtual Ref<Syntax > const ParenthesisMatch(int level, SourcePart const&part)const {
            bool Trace = true;
            md(level, part);
            mb;
        };

        AssumeConstObject;
        Ref<CodeItem> const Code(Context const&context)const;
        WeakRef<Type>const Type(Context const&context)const;
        Ref<ResultFromSyntaxAndContext> const GetResultCache(Context const&context)const;
        virtual ResultData const GetResultData(Context const&context, Category category)const;

        Ref<Syntax> const ReplaceArg(Ref<Syntax> const&arg)const;
        virtual Ref<Syntax,true> Replace(SyntaxArgVisitor const&visitor)const;
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
        p_function(Array<String>,DumpData) override;
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
        p_function(Array<String>,DumpData) override;
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
        p_function(Array<String>,DumpData) override;
    };


};
