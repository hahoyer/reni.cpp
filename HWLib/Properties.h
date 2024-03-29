#pragma once

#define HW_PR_GETTER_NAME(NAME) get_##NAME
#define HW_PR_SETTER_NAME(NAME) set_##NAME

#define HW_PR_GETTER_SPECIFICATION(TYPE, NAME) _declspec ( property ( get = HW_PR_GETTER_NAME(NAME)) ) TYPE NAME
#define HW_PR_SETTER_SPECIFICATION(TYPE, NAME) _declspec ( property ( put = HW_PR_SETTER_NAME(NAME)) ) TYPE NAME
#define HW_PR_VARIABLE_SPECIFICATION(TYPE, NAME) _declspec ( property ( get = HW_PR_GETTER_NAME(NAME), put = HW_PR_SETTER_NAME(NAME)) ) TYPE NAME

// inline implementations
#define HW_PR_DECL_GETTER(TYPE, NAME) TYPE HW_PR_GETTER_NAME(NAME)()const
#define HW_PR_DECL_MUTABLE_GETTER(TYPE, NAME) TYPE HW_PR_GETTER_NAME(NAME)()
#define HW_PR_DECL_SETTER(TYPE, NAME) void HW_PR_SETTER_NAME(NAME)(TYPE const&value)

// implementations outside class
#define HW_PR_IMPL_GETTER(CLASS, TYPE, NAME) TYPE CLASS::HW_PR_GETTER_NAME(NAME)()const
#define HW_PR_IMPL_MUTABLE_GETTER(CLASS, TYPE, NAME) TYPE CLASS::HW_PR_GETTER_NAME(NAME)()
#define HW_PR_IMPL_SETTER(CLASS, TYPE, NAME) void CLASS::HW_PR_SETTER_NAME(NAME)(TYPE const&value)

// specification and function declaration, getter function is at end
#define HW_PR_GET(TYPE, NAME) HW_PR_GETTER_SPECIFICATION(TYPE,NAME); HW_PR_DECL_GETTER(TYPE,NAME)
#define HW_PR_MUTABLE_GET(TYPE, NAME) HW_PR_GETTER_SPECIFICATION(TYPE,NAME); HW_PR_DECL_MUTABLE_GETTER(TYPE,NAME)
#define HW_PR_VAR(TYPE, NAME) HW_PR_VARIABLE_SPECIFICATION(TYPE,NAME); HW_PR_DECL_SETTER(TYPE,NAME); HW_PR_DECL_GETTER(TYPE,NAME)
// specification and function declaration, setter function is at end
#define HW_PR_SET(TYPE, NAME) HW_PR_SETTER_SPECIFICATION(TYPE,NAME); HW_PR_DECL_SETTER(TYPE,NAME)
#define HW_PR_VAR_SETTER(TYPE, NAME) HW_PR_VARIABLE_SPECIFICATION(TYPE,NAME); HW_PR_DECL_GETTER(TYPE,NAME); HW_PR_DECL_SETTER(TYPE,NAME)

// Virtual properties
#define HW_PR_VIRTUAL_GETTER_NAME(NAME) virtual_get_##NAME
#define HW_PR_VIRTUAL_SETTER_NAME(NAME) virtual_set_##NAME

#define HW_PR_VIRTUAL_GETTER_SPECIFICATION(TYPE, NAME) _declspec ( property ( get = HW_PR_VIRTUAL_GETTER_NAME(NAME)) ) TYPE NAME
#define HW_PR_VIRTUAL_SETTER_SPECIFICATION(TYPE, NAME) _declspec ( property ( put = HW_PR_VIRTUAL_SETTER_NAME(NAME)) ) TYPE NAME
#define HW_PR_VIRTUAL_VARIABLE_SPECIFICATION(TYPE, NAME) _declspec ( property ( get = HW_PR_VIRTUAL_GETTER_NAME(NAME), put = HW_PR_VIRTUAL_SETTER_NAME(NAME)) ) TYPE NAME

#define HW_PR_DECL_VIRTUAL_GETTER(TYPE, NAME) TYPE HW_PR_VIRTUAL_GETTER_NAME(NAME)()const
#define HW_PR_DECL_VIRTUAL_SETTER(TYPE, NAME) void HW_PR_VIRTUAL_SETTER_NAME(NAME)(TYPE const&value)

/// <summary>
///  Declare a wrapper method and its virtual kernel.
/// </summary>
/// <remarks>Scope will change to protected.</remarks>
/// <param name="TYPE">Type of the property defined.</param>
/// <param name="NAME">Name of the property defined.</param>
#define HW_PR_VIRTUAL_GET(TYPE, NAME) HW_PR_VIRTUAL_GETTER_SPECIFICATION(TYPE,NAME);\
    HW_PR_DECL_VIRTUAL_GETTER(TYPE,NAME);\
    protected:\
    virtual HW_PR_DECL_GETTER(TYPE,NAME)

/// <summary>
///  Declare a wrapper method with inline implementation and its virtual kernel.
/// </summary>
/// <remarks>Scope will change to protected.</remarks>
/// <param name="TYPE">Type of the property defined.</param>
/// <param name="NAME">Name of the property defined.</param>
#define HW_PR_VIRTUAL_GET_INLINE(TYPE, NAME) HW_PR_VIRTUAL_GETTER_SPECIFICATION(TYPE,NAME);\
    HW_PR_DECL_VIRTUAL_GETTER(TYPE,NAME){ return HW_PR_GETTER_NAME(NAME)(); };\
    protected:\
    virtual HW_PR_DECL_GETTER(TYPE,NAME)

#define HW_PR_VIRTUAL_GETTER_WRAPPER(CLASS, TYPE, NAME) TYPE CLASS::HW_PR_VIRTUAL_GETTER_NAME(NAME)()const { return HW_PR_GETTER_NAME(NAME)(); };
#define HW_PR_VIRTUAL_SETTER_WRAPPER(CLASS, TYPE, NAME) void CLASS::HW_PR_VIRTUAL_SETTER_NAME(NAME)(TYPE const&value){HW_PR_SETTER_NAME(NAME)(value);}

// convenience macros
#define HW_PR_BASE_GETTER_NAME(NAME) baseType::get_##NAME()
#define HW_PR_THISREF HW_PR_MUTABLE_GET(thisType&, thisRef)const{return const_cast<thisType&>(*this);}

#include "_EditorTemplates.h"
