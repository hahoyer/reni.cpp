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

        Array<String> const p_name(DumpData)()const override;

        ResultData const GetResultData(Category category)const override;
        p(Ref<CodeItem>, codeGet);
        p(Ref<CodeItem>, CodeSet);
        p(WeakRef<Type>, valueType);
    };
};
