#pragma once
#include "Feature.h"

using namespace HWLib;

namespace Reni
{
    class AccessType;
    class EnableCutType;
    class NumberType;
    template<class TFeature> class SearchResult;
    class TypeType;

    class DeclarationType
    {
    public:
        virtual SearchResult<Feature> const Declarations(NumberType const&) const = 0;
        virtual SearchResult<Feature> const Declarations(TypeType const&) const = 0;
        virtual SearchResult<Feature> const Declarations(EnableCutType const&) const = 0;
        virtual SearchResult<Feature> const Declarations(AccessType const&) const = 0;

        virtual ~DeclarationType()
        {
        };

    };
}