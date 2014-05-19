#pragma once
#include "../HWLib/Ref.h"
#include "../HWLib/DumpableObject.h"

using namespace HWLib;

namespace Reni{
    class ExpressionSyntax;
    class ResultData;
    class Context;
    class Feature;
    class NumberType;
    class TypeType;

    class SearchResult : public DumpableObject{
        using baseType = DumpableObject;
        using thisType = SearchResult;
    public:
        Ref<Feature,true> const feature;
        SearchResult(Ref<Feature> feature);
        SearchResult();

        p(bool, IsValid);
    private:
        p_function(Array<String>,DumpData) override;
    };


    struct SearchTarget
    {
        virtual SearchResult const Search(NumberType const&) const = 0;
        virtual SearchResult const Search(TypeType const&) const = 0;
        virtual ~SearchTarget(){};
    };
}