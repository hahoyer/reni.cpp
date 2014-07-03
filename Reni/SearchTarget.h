#pragma once
#include "Feature.h"

using namespace HWLib;

namespace Reni
{
    class NumberType;
    class TypeType;
    class EnableCutType;
    template<class TFeature> class SearchResult;
    
    class SearchTarget
    {
    public:
        virtual SearchResult<Feature> const Search(NumberType const&) const = 0;
        virtual SearchResult<Feature> const Search(TypeType const&) const = 0;

        virtual ~SearchTarget()
        {
        };

    };
}