#pragma once
#include "PlacementNew.h"
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>

#define HW_DO_PLACEMENT_ASSIGN thisType& operator=(thisType const&x)\
{\
	if(this == &x) return *this;\
    this->~thisType(); \
    return *new (this) thisType(x); \
};

#define  HW_DO_DECL_CC(type)type(type const&)
#define  HW_DO_DECL_MC(type)type(type &&)
#define  HW_DO_DECL_CA(type)type& operator=(type const&)
#define  HW_DO_DECL_MA(type)type& operator=(type &&)
#define  HW_DO_DECL_D(type)~type()

#define HW_DO_OPERATOR(type,name)HW_DO_DECL_##name(type)
#define HW_DO_DELETE_I(r,type,n,name)HW_DO_OPERATOR(type,name) = delete;
#define HW_DO_DEFAULT_I(r,type,n,name)HW_DO_OPERATOR(type,name) = default;

#define HW_DO_DELETE(type, ...)BOOST_PP_SEQ_FOR_EACH_I(HW_DO_DELETE_I, type, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))
#define HW_DO_DEFAULT(type, ...)BOOST_PP_SEQ_FOR_EACH_I(HW_DO_DEFAULT_I, type, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

#define HW_DO_DELETE_ALL(type)HW_DO_DELETE(type, CC,MC,CA,MA,D)
#define HW_DO_DEFAULT_ALL(type)HW_DO_DEFAULT(type, CC,MC,CA,MA,D)
