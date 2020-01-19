#pragma once
#include "PlacementNew.h"
#define DefaultAssignmentOperator thisType& thisType::operator=(thisType const&x)\
{\
	if(this == &x) return *this;\
    this->~thisType(); \
    return *new (this) thisType(x); \
};

