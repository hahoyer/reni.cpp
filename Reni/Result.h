#pragma once
#include "ResultData.h"
#include "../HWLib/Pod.h"

using namespace HWLib;
using namespace Util;


namespace Reni
{
  class ResultCache
    : public DumpableObject
      , public RefCountProvider
  {
    typedef DumpableObject baseType;
    typedef ResultCache thisType;

  protected:
    mutable ResultData data;
    mutable Category pending;

  public:
    pod<bool> Trace;

    ResultCache();

    HW_PR_THISREF;
    HW_PR_GET(bool, hollow);
    HW_PR_GET(Size, size);
    HW_PR_GET(Ref<CodeItem >, code);
    HW_PR_GET(WeakRef<Type>, type);
    HW_PR_GET(Closure, closure);
    HW_PR_GET(Optional<WeakRef<Type>>, cachedType);

    ResultData operator &(const Category& category) const { return Get(category); }
    ResultData Get(const Category& category) const;

  private:
    void Ensure(const Category& category) const;

  protected:
    HW_PR_DECL_GETTER(Array<string>, DumpData) override;
    HW_PR_VIRTUAL_GET(bool, isRecursion) { return false; };
    virtual ResultData GetResultData(const Category& category) const = 0;
    virtual ResultData GetResultDataRecursive(const Category& category) const;

  private:
    HW_PR_GET(Category, complete);
  };


  class Syntax;
  class Context;

  class ResultFromSyntaxAndContext final : public ResultCache
  {
    typedef ResultCache baseType;
    typedef ResultFromSyntaxAndContext thisType;

  public:
    const Syntax& syntax;
    const Context& context;
    ResultFromSyntaxAndContext(const Syntax& syntax, const Context& context);

  private:
    HW_PR_DECL_GETTER(Array<string>, DumpData) override;
    HW_PR_DECL_GETTER(bool, isRecursion) override;
    ResultData GetResultData(const Category& category) const override;
    ResultData GetResultDataRecursive(const Category& category) const override;
  };
}
