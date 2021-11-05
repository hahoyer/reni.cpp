#pragma once
#include "Ref.h"

#include "DefaultOperators.h"
#include "RefBase.h"

using namespace HWLib;

template<typename T>
RefCountContainer<T>::RefCountContainer(T *value) :value(value){
    if (value)
        value->AddReference();
};

template<typename T>
RefCountContainer<T>::RefCountContainer(RefCountContainer<T> const&other) :value(other.value) {
    if(value)
        value->AddReference();
};

template<typename T>
RefCountContainer<T>::~RefCountContainer(){
    if (value)
        value->RemoveReference();
};