#pragma once

#include "Result.h"

using namespace HWLib;

namespace Reni
{
    class ContainerContext;

    class AccessResultCache final : public ResultCache
    {
        typedef ResultCache baseType;
        typedef AccessResultCache thisType;
        int const statementIndex;
        Ref<ResultCache> dataResultCache;
    public:
        ContainerContext const& container;

        AccessResultCache(ContainerContext const& container, int statementIndex);
    private:
        p_function(Array<String>, DumpData) override{ return{nd(statementIndex), nd(container)}; }

        ResultData const GetResultData(Category category)const override;
        p(Ref<CodeItem>, codeGet);
        p(Ref<CodeItem>, CodeSet);
        p(WeakRef<Type>, valueType);
    };
};
