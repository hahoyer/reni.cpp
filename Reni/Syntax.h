#pragma once

#include "Result.h"
#include "..\HWLang\SourcePart.h"
#include "..\HWLib\FunctionCache.h"
#include "Context.h"
#include "../Util/Size.h"
using namespace HWLib;
using namespace HWLang;

namespace Reni
{
  class CodeItem;
  class Context;
  class Closure;
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
    FunctionCache<Ref<ResultFromSyntaxAndContext>, const Context*> resultCache;
  protected:
    const SourcePart part;
    Syntax(const SourcePart& part);
  public:
    Syntax(const Syntax&) = delete;

    virtual const Ref<Syntax> ParenthesisMatch(int level, const SourcePart& part) const
    {
      const bool Trace = true;
      HW_D_METHOD(level, part);
      HW_BREAK_AND_THROW;
    };

    ThisRef;
    Optional<WeakRef<Type>> CachedType(const RegularContext& context) const;

    Size GetSize(const Context& context) const;
    Ref<CodeItem> GetCode(const Context& context) const;
    WeakRef<Type> GetType(const Context& context) const;
    Closure GetClosure(const Context& context) const;

    Ref<ResultFromSyntaxAndContext> GetResultCache(const Context& context) const;

    Ref<SyntaxContainer> Defines(const SourcePart& part, const Ref<Syntax>& value) const;
    Ref<Syntax> ReplaceArg(const Ref<Syntax>& arg) const;
    virtual Optional<Ref<Syntax>> Replace(const SyntaxArgVisitor& visitor) const;
    virtual void AddTo(SyntaxContainer& syntaxContainer) const;
    Ref<Syntax> TypeOperator(const SourcePart& part) const;

    p_virtual(int, priority) = 0;
    string SmartDumpFrame(int priority) const;
  protected:
    virtual ResultData GetResultData(const Context& context, Category const& category) const;
    p_virtual(string, SmartDump) = 0;
    friend class ResultFromSyntaxAndContext;
  private:
    p_function(Array<string>, DumpData) final;
  };


  class TokenClass;


  class InfixSyntax : public Syntax
  {
    using baseType = Syntax;
    const Ref<Syntax> left;
    const TokenClass& tokenClass;
    const Ref<Syntax> right;
  public:
    InfixSyntax(const Ref<Syntax> left, const TokenClass& tokenClass, const SourcePart part, const Ref<Syntax> right)
      : baseType(part)
        , left(left)
        , tokenClass(tokenClass)
        , right(right)
    {
      SetDumpString();
    }

  private:
    p_function(string, SmartDump) override;
  };


  class PrefixSyntax : public Syntax
  {
    using baseType = Syntax;
    const TokenClass& tokenClass;
    const Ref<Syntax> right;
  public:
    PrefixSyntax(const TokenClass& tokenClass, const SourcePart part, const Ref<Syntax> right)
      : baseType(part)
        , tokenClass(tokenClass)
        , right(right)
    {
      SetDumpString();
    }

  private:
    p_function(string, SmartDump) override;
  };


  class SuffixSyntax : public Syntax
  {
    using baseType = Syntax;
    const Ref<Syntax> left;
    const TokenClass& tokenClass;
  public:
    SuffixSyntax(const Ref<Syntax> left, const TokenClass& tokenClass, const SourcePart part)
      : baseType(part)
        , left(left)
        , tokenClass(tokenClass)
    {
      SetDumpString();
    }

  private:
    p_function(string, SmartDump) override;
  };
};
