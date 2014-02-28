#pragma once
#include "..\HWLib\DumpableObject.h"

using namespace HWLib;

namespace Reni
{
    class ExpressionSyntax;
    class ResultData;
    class Context;
    class Category;

    class SearchResult : public HWLib::DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = SearchResult;
    public:
        p(bool, IsValid);
        ResultData const FunctionResult(Context const&context, Category category, ExpressionSyntax const& expressionSyntax)const;
    private:
        override_p_function(Array<String>, DumpData){
            return{ };
        };
    };
}
