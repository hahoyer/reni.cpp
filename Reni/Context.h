#pragma once

#include "External.h"

#include "../HWLib/RefCountProvider.h"
#include "../Util/Category.h"
#include "../HWLib/WeakRef.h"
#include "../HWLib/Ref.h"
#include "../HWLib/Optional.h"

using namespace HWLib;
using namespace Util;

namespace Reni
{
  class AccessFeature;
  class ContainerContext;
  template <class T>
  class ContextFeatureProvider;
  class DefineableToken;
  class FunctionBodyType;
  class FunctionCallContext;
  class FunctionSyntax;
  class Global;
  class MinusToken;
  class RecursionContext;
  class ResultData;
  class RootContext;
  template <class T>
  class SearchResult;
  class Syntax;
  class SyntaxContainer;
  class Type;

  class Context
    : public WithId<DumpableObject, Context>
  {
    using baseType = WithId<DumpableObject, Context>;
    using thisType = Context;

  public:
    HW_PR_VIRTUAL_GET(bool, isRecursion) { return false; };

  public:
    HW_PR_VIRTUAL_GET(WeakRef<Global>, global) = 0;

  public:
    HW_PR_VIRTUAL_GET(WeakRef<FunctionCallContext>, functionContext) = 0;

  public:
    HW_PR_VIRTUAL_GET(WeakRef<RecursionContext>, recursionContext) = 0;

  public:
    HW_PR_GET(int, alignBits) { return 3; };

    virtual WeakRef<Type> const FunctionType(FunctionSyntax const& body) const = 0;
    virtual ResultData const ReferenceResult(Category category, External::Function const& external) const;
    virtual SearchResult<AccessFeature> const DeclarationsForType(DefineableToken const& token) const;
    virtual WeakRef<ContainerContext> const Container(SyntaxContainer const& statements, size_t viewIndex) const = 0;
  };

  class RegularContext : public Context
  {
    using baseType = Context;
    using thisType = RegularContext;

    class internal;
    WeakRef<internal> _internal;

    RegularContext(RegularContext const&) = delete;

  protected:
    RegularContext();

  public:
    WeakRef<ContainerContext> const Container(SyntaxContainer const& syntax, size_t index) const override;
    WeakRef<Type> const FunctionType(FunctionSyntax const& body) const override;

    virtual operator Optional<Ref<ContextFeatureProvider<MinusToken>>>() const;
    virtual operator Optional<Ref<ContextFeatureProvider<DefineableToken>>>() const;

    template <class T>
    SearchResult<AccessFeature> const GetGenericDefinition() const
    {
      Ref<ContextFeatureProvider<T>> f = *this;
      Ref<AccessFeature> ff = f->feature;
      return *ff;
    };

  private:
    HW_PR_DECL_GETTER(WeakRef<RecursionContext>, recursionContext) override;
  };

  class ChildContext : public RegularContext
  {
    using baseType = RegularContext;
    using thisType = ChildContext;

    explicit ChildContext(ChildContext const&) = delete;

  protected:
    RegularContext const& parent;

    ChildContext(RegularContext const& parent)
      : parent(parent)
    { }

    HW_PR_DECL_GETTER(Array<string>, DumpData) override { return {HW_D_VALUE(parent)}; }

    SearchResult<AccessFeature> const DeclarationsForType(DefineableToken const& token) const override;;

  private:
    HW_PR_DECL_GETTER(WeakRef<Global>, global) override { return parent.global; };
    HW_PR_DECL_GETTER(WeakRef<FunctionCallContext>, functionContext) override { return parent.functionContext; };
  };
}

using namespace Reni;

class External::Context final : public External
{
  using baseType = External;
  using thisType = Context;
  Reni::Context const& target;

public:
  Context(Reni::Context const& target)
    : target(target)
  {
    SetDumpString();
  }

  string internalDump() const override;

private:
  Closure const Replace(ReplaceVisitor const& arg) const override;
};


namespace std
{
  template <>
  struct hash<Context const&>
  {
    size_t operator()(Context const& key) const
    {
      return key.ObjectId;
    }
  };
}
