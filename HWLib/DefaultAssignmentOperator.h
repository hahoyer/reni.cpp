#pragma once
#include "PlacementNew.h"
#define DefaultAssignmentOperator(cType)cType& cType::operator=(cType const&x)\
{\
	if(this == &x) return *this;\
	this->~cType();\
	return *new (this) cType(x);\
};

