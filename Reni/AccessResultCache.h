#pragma once
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
        AccessResultCache(AccessData const&data);

        Array<string> const p_name(DumpData)()const override;

        ResultData GetResultData(Category const& category)const override;
        p(Ref<CodeItem>, codeGet);
        p(Ref<CodeItem>, CodeSet);
        p(WeakRef<Type>, valueType);
    };
};
