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
        virtual Ref<FeatureProvider<T, TTypes...>, true>const Convert(T0 const&top)const = 0;
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


    template<class TTokenClass, class TTargetTypeHandler>
    class ArglessFunctionProvider final
        : public FeatureProvider<TTokenClass>
    {
        typedef FeatureProvider<TTokenClass> baseType;
        typedef ArglessFunctionProvider thisType;
        typedef typename TTargetTypeHandler::targetType targetType;
        class Feature;
        Ref<Feature> feature;
    public:
        ArglessFunctionProvider(targetType const&value);
        AssumeConstObject;
    private:
        p_function(Array<String>, DumpData) override;
        p_function(Ref<Reni::Feature>, feature) override;
    };


    template<class TTokenClass, class TTargetTypeHandler>
    class InfixFunctionProvider final
        : public FeatureProvider<TTokenClass>
    {
        typedef FeatureProvider<TTokenClass> baseType;
        typedef InfixFunctionProvider thisType;
        typedef typename TTargetTypeHandler::targetType targetType;
        class Feature;
        Ref<Feature> feature;
    public:
        InfixFunctionProvider(targetType const&value);
        AssumeConstObject;
    private:
        p_function(Array<String>, DumpData) override;
        p_function(Ref<Reni::Feature>, feature) override;
    };


}

