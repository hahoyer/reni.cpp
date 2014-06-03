#pragma once

#include "Result.h"
#include "..\HWLang\SourcePart.h"
#include "..\HWLib\FunctionCache.h"
#include "Context.h"
using namespace HWLib;
using namespace HWLang;

namespace Reni
{
    class CodeItem;
    class Context;
    class Link;
    class RegularContext;
    class ResultCache;
    class ResultFromSyntaxAndContext;
    class SyntaxArgVisitor;
    class SyntaxContainer;
    class TokenClass;
    class Type;

    class Syntax 
        : public WithId<DumpableObject, Syntax>
        , public RefCountProvider
    {
        using baseType = WithId<DumpableObject, Syntax>;
        using thisType = Syntax;
    public:
        using TokenClass = TokenClass;

    private:
        FunctionCache<Ref<ResultFromSyntaxAndContext>, Context const*> resultCache;
    protected:
        SourcePart const part;
        Syntax(SourcePart const& part);
        Syntax(Syntax const& ) = delete;
    public:
        virtual Ref<Syntax > const ParenthesisMatch(int level, SourcePart const&part)const {
            bool Trace = true;
            md(level, part);
            mb;
        };

        ThisRef;
        WeakPtr<Type> const CachedType(RegularContext const& context) const;

        Size const Size(Context const&context)const;
        Ref<CodeItem> const Code(Context const&context)const;
        WeakRef<Type> const Type(Context const&context)const;
        Array<Ref<Link>> const Links(Context const&context) const;

        Ref<ResultFromSyntaxAndContext> const GetResultCache(Context const&context)const;

        Ref<SyntaxContainer> const Defines(SourcePart const& part, Ref<Syntax> const&value)const;
        Ref<Syntax> const ReplaceArg(Ref<Syntax> const&arg)const;
        virtual Ref<Syntax,true> const Replace(SyntaxArgVisitor const&visitor)const;
        virtual void AddTo(SyntaxContainer& syntaxContainer) const;
        Ref<Syntax> const TypeOperator(SourcePart const part) const;

        virtual_p(int, priority) = 0;
        String const SmartDumpFrame(int priority)const;
    protected:
        virtual ResultData const GetResultData(Context const&context, Category category)const;
        virtual_p(String, SmartDump) = 0;
        friend class ResultFromSyntaxAndContext;
    private: 
        p_function(Array<String>, DumpData) override final;
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
        p_function(String,SmartDump) override;
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
        p_function(String, SmartDump) override;
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
        p_function(String, SmartDump) override;
    };


};
