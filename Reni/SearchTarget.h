#pragma once
#include "Feature.h"

using namespace HWLib;

namespace Reni
{
    class NumberType;
    class TypeType;
    class EnableCutType;
    template<class TFeature> class SearchResult;
    
    class DeclarationType
    {
    public:
        virtual SearchResult<Feature> const Declarations(NumberType const&) const = 0;
        virtual SearchResult<Feature> const Declarations(TypeType const&) const = 0;
        virtual SearchResult<Feature> const Declarations(EnableCutType const&) const = 0;

        virtual ~DeclarationType()
        {
        };

    };
}