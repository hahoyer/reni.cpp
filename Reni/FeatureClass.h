#pragma once
#include "../HWLib/DumpableObject.h"
#include "DefinitionPoint.h"
#include "SearchResult.h"

using namespace HWLib;

namespace Reni{
    class ArrayType;
    class Type;

    
    class FeatureClass : public WithId<DumpableObject, FeatureClass>, public RefCountProvider{
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

    
    template <typename T, typename... TTypes>
    class FeatureProvider : public RefCountProvider{
        using thisType = FeatureProvider;
    public:
        virtual ~FeatureProvider(){};
    };


    template <typename T, typename T0, typename... TTypes>
    class FeatureProvider<T, T0, TTypes...> : public RefCountProvider{
        using thisType = FeatureProvider;
    public:
        virtual Ptr<FeatureProvider<T, TTypes...>>const Convert(T0 const&top)const = 0;
        virtual ~FeatureProvider(){};
    };
}


