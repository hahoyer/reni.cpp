#pragma once
#include "../HWLib/DumpableObject.h"
#include "../HWLib/RefCountProvider.h"
#include "../HWLib/Ref.h"

using namespace HWLib;

namespace Reni{
    class Feature;


    template <typename T, typename... TTypes>
    class FeatureProvider
        : public DumpableObject
        , public RefCountProvider {
        using thisType = FeatureProvider;
    public:
        virtual ~FeatureProvider(){};
    };


    template <typename T, typename T0, typename... TTypes>
    class FeatureProvider<T, T0, TTypes...>
        : public DumpableObject
        , public RefCountProvider{
        using thisType = FeatureProvider;
    public:
        virtual Ptr<FeatureProvider<T, TTypes...>>const Convert(T0 const&top)const = 0;
        virtual ~FeatureProvider(){};
    };


    template <typename T>
    class FeatureProvider<T>
        : public DumpableObject
        , public RefCountProvider{
        using thisType = FeatureProvider;
    public:
        virtual_p(Ref<Feature>, feature) = 0;
        virtual ~FeatureProvider(){};
    };
}

