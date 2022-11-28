#pragma once
#include "Result.h"
using namespace HWLib;


namespace Reni
{
    class FunctionResultCache final : public ResultCache
    {
        typedef ResultCache baseType;
        typedef FunctionResultCache thisType;
        function<ResultData(Category)> const getData;
    public:
        FunctionResultCache(function<ResultData(Category)> getData)
            : getData(getData)
        {
            SetDumpString();
        }

    private:
        HW_PR_DECL_GETTER(Array<string>,DumpData) override
        {
            return{};
        };

        ResultData GetResultData(const Category &category) const override
        {
            return getData(category);
        }
    };
}