#pragma once
#include "../HWLib/DumpableObject.h"
#include "../HWLib/RefCountProvider.h"
#include "../HWLib/Ref.h"

using namespace HWLib;

namespace Reni{
    class AccessFeature;


    template <typename T, typename... TTypes>
    class ContextFeatureProvider
        : public DumpableObject
        , public RefCountProvider {
        typedef ContextFeatureProvider thisType;
    public:
        virtual ~ContextFeatureProvider(){};
    };


    template <typename T, typename T0, typename... TTypes>
    class ContextFeatureProvider <T, T0, TTypes...>
        : public DumpableObject
        , public RefCountProvider{
        typedef ContextFeatureProvider thisType;
    public:
        virtual Optional<Ref<ContextFeatureProvider<T, TTypes...>>>const Convert(T0 const&top)const = 0;
        virtual ~ContextFeatureProvider(){};
    };


    template <typename T>
    class ContextFeatureProvider <T>
        : public DumpableObject
        , public RefCountProvider{
        typedef ContextFeatureProvider thisType;
    public:
        virtual_p(Ref<AccessFeature>, feature) = 0;
        virtual ~ContextFeatureProvider(){};
    };
}

