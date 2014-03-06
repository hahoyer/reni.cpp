#pragma once
#include "DumpPrintToken.h"
#include "SearchResult.h"

using namespace Reni;
using namespace HWLib;


template <typename T>
SearchResult const GetGenericDefinition(Type const&type){
    fd(type);
    b_;
    return{};
}

template <typename T>
SearchResult const GenericFeatureClass<T>::GetDefinition(Type const&type)const{ return GetGenericDefinition<T>(type); }

template <>
SearchResult const GetGenericDefinition<DumpPrintToken>(Type const&type){
    CtrlPtr<FeatureProvider<DumpPrintToken>> f = type;

    fd(type, f);
    b_;
    return{};
}


