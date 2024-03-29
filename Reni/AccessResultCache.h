﻿#pragma once
#include "Result.h"

using namespace HWLib;

namespace Reni
{
  class AccessData;

  class AccessResultCache final : public ResultCache
  {
    typedef ResultCache baseType;
    typedef AccessResultCache thisType;

    Ref<AccessData> data;

  public:
    AccessResultCache(AccessData const& data);

    virtual HW_PR_DECL_GETTER(Array<string>, DumpData) override;

    /// <summary>
    /// 
    /// </summary>
    /// <param name="category"></param>
    /// <returns></returns>
    ResultData GetResultData(Category const& category) const override;
    HW_PR_GET(Ref<CodeItem>, codeGet);
    HW_PR_GET(Ref<CodeItem>, CodeSet);
    HW_PR_GET(WeakRef<Type>, valueType);
  };
};
