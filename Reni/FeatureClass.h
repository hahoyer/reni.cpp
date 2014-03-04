#pragma once
#include "../HWLib/DumpableObject.h"
#include "DefinitionPoint.h"
#include "SearchResult.h"

using namespace HWLib;

namespace Reni{
    class ArrayType;

    class FeatureClass : public WithId<DumpableObject, FeatureClass>{
        using baseType = WithId<DumpableObject, FeatureClass >;
        using thisType = FeatureClass;
    public:
        ref_p;
        virtual SearchResult const GetDefinition(Type const&type)const = 0;
    private:
        override_p_function(Array<String>, DumpData){ return{}; }
    };

    template <typename T>
    class GenericFeatureClass : public FeatureClass {
        using baseType = FeatureClass;
        using thisType = GenericFeatureClass;
        virtual SearchResult const GetDefinition(Type const&type)const override;
    };

    template <typename T>
    class FeatureProvider {
        using thisType = FeatureProvider;

    public:
        virtual ~FeatureProvider(){};
    };


}

