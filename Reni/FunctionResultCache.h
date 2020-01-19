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
        p_function(Array<String>, DumpData) override
        {
            return{};
        };

        ResultData const GetResultData(Category category)const override
        {
            return getData(category);
        }
    };
}