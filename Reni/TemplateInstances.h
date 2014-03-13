#pragma once
#include "DumpPrintToken.h"
#include "FeatureProvider.h"
#include "SearchResult.h"

using namespace Reni;
using namespace HWLib;


template <typename T>
inline SearchResult const GetGenericDefinition(Type const&type){
    fd(type);
    mb;
}

template <typename T>
inline SearchResult const GetGenericDefinition(Context const&context){
    fd(context);
    mb;
}

template <typename T>
inline SearchResult const GenericFeatureClass<T>::GetDefinition(Type const&type)const{
    return GetGenericDefinition<T>(type);
}

template <typename T>
inline SearchResult const GenericFeatureClass<T>::GetDefinition(Context const&context)const{
    return GetGenericDefinition<T>(context);
}

template <>
inline SearchResult const GetGenericDefinition<DumpPrintToken>(Type const&type){
    Ref<FeatureProvider<DumpPrintToken>, true> f = type;
    return f->feature;
}

template <>
inline SearchResult const GetGenericDefinition<SignToken>(Context const&context){
    fd(context);
    mb;
}

template <typename T>
pure_p_implementation(FeatureProvider<T>, Ref<Feature>, feature);
