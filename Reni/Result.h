#pragma once
#include "ResultData.h"
#include "../HWLib/Pod.h"

using namespace HWLib;
using namespace Util;
using namespace std;


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

    ThisRef;
    p(bool, hollow);
    p(Size, size);
    p(Ref<CodeItem >, code);
    p(WeakRef<Type>, type);
    p(Closure, closure);
    p(Optional<WeakRef<Type>>, cachedType);

    ResultData operator &(const Category& category) const { return Get(category); }
    ResultData Get(const Category& category) const;
  private:
    void Ensure(const Category& category) const;
  protected:
    p_function(Array<string>, DumpData) override;
    p_virtual(bool, isRecursion) { return false; };
    virtual ResultData GetResultData(const Category& category) const = 0;
    virtual ResultData GetResultDataRecursive(const Category& category) const;
  private:
    p(Category, complete);
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
    p_function(Array<string>, DumpData) override;
    p_function(bool, isRecursion) override;
    ResultData GetResultData(const Category& category) const override;
    ResultData GetResultDataRecursive(const Category& category) const override;
  };
}
