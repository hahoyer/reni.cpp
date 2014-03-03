#pragma once
#include "../HWLib/DumpableObject.h"
#include "../HWLib/Ref.h"

using namespace HWLib;

namespace Reni{
    class SearchResult;

    class FeatureClass : public WithId<DumpableObject, FeatureClass>{
        using baseType = WithId<DumpableObject, FeatureClass >;
        using thisType = FeatureClass;
    public:
        ref_p;
    private:
        override_p_function(Array<String>, DumpData){ return{}; };
    };

    class DefinitionPoint : public WithId<DumpableObject, DefinitionPoint>{
        using baseType = WithId<DumpableObject, DefinitionPoint>;
        using thisType = DefinitionPoint;
    public:
        bool const Defines(FeatureClass const&feature)const;
        SearchResult const Apply(FeatureClass const&feature)const;
        ref_p;
    private:
        override_p_function(Array<String>, DumpData){ return{}; };
    };

    template <typename T>
    class GenericDefinitionPoint : public DefinitionPoint{
        using baseType = DefinitionPoint;
        using thisType = GenericDefinitionPoint;
    public:
        bool const Defines(FeatureClass const&feature)const;
        SearchResult const Apply(FeatureClass const&feature)const;
        ref_p;
    private:
        override_p_function(Array<String>, DumpData){ return{}; };
    };

}

