#pragma once
#include "../HWLib/Ref.h"
#include "../HWLib/DumpableObject.h"

using namespace HWLib;

namespace Reni{
    class ExpressionSyntax;
    class ResultData;
    class Context;
    class Category;
    class Feature;

    class SearchResult : public DumpableObject{
        using baseType = DumpableObject;
        using thisType = SearchResult;
        Ptr<Feature> feature;
    public:
        SearchResult(Ref<Feature> feature);
        SearchResult() = default;

        p(bool, IsValid);
        ResultData const FunctionResult(
            Context const&context,
            Category category,
            ExpressionSyntax const& expressionSyntax
        )const;
    private:
        override_p_function(Array<String>, DumpData);
    };
}