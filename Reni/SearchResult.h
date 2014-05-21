#pragma once
#include "../HWLib/Ref.h"
#include "../HWLib/DumpableObject.h"
#include "Feature.h"

using namespace HWLib;

namespace Reni
{
    class ExpressionSyntax;
    class ResultData;
    class Context;
    class NumberType;
    class TypeType;

    template<class TFeature>
    class SearchResult : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = SearchResult;
    public:
        TFeature const feature;

        SearchResult(TFeature const&feature) : feature(feature)
        {
            SetDumpString();
        }

        SearchResult()
        {
            SetDumpString();
        }


        p(bool, IsValid)
        {
            return !feature.isEmpty;
        };

    private:
        p_function(Array<String>, DumpData) override
        {
            return{nd(feature)};
        };
    };


    template<class TFeature>
    struct SearchTarget
    {
        virtual SearchResult<TFeature> const Search(NumberType const&) const = 0;
        virtual SearchResult<TFeature> const Search(TypeType const&) const = 0;

        virtual ~SearchTarget()
        {
        };
    };
}