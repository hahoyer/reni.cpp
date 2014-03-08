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
    public:
        Ref<Feature,true> const feature;
        SearchResult(Ref<Feature> feature);
        SearchResult() = default;

        p(bool, IsValid);
    private:
        override_p_function(Array<String>, DumpData);
    };
}