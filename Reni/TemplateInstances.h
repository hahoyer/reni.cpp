#pragma once
#include "DumpPrintToken.h"
#include "FeatureProvider.h"
#include "SearchResult.h"

using namespace Reni;
using namespace HWLib;


template <typename T>
inline SearchResult const GenericFeatureClass<T>::GetDefinition(Type const&type)const{
    md(type);
    mb;
}

template <typename T>
inline SearchResult const GenericFeatureClass<T>::GetDefinition(Context const&context)const{
    md(context);
    mb;
}

#define TypeFeature(tokenClass)\
template <>\
    inline SearchResult const GenericFeatureClass<tokenClass>::GetDefinition(Type const&target)const {\
    return target.GetGenericDefinition<tokenClass>();\
}

#define ContextFeature(tokenClass)\
    template <>\
    inline SearchResult const GenericFeatureClass<tokenClass>::GetDefinition(Context const&target)const {\
    return target.GetGenericDefinition<tokenClass>();\
}

TypeFeature(DumpPrintToken);
ContextFeature(MinusToken);

template <typename T>
pure_p_implementation(FeatureProvider<T>, Ref<Feature>, feature);
template <typename T>
pure_p_implementation(ContextFeatureProvider<T>, Ref<ContextFeature>, feature);
