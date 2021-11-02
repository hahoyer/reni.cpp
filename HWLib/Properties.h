#pragma once

#define p_name(NAME) get_##NAME
#define p_mutator_name(NAME) set_##NAME

#define p_definition(TYPE, NAME) _declspec ( property ( get = p_name(NAME)) ) TYPE NAME
#define p_mutator_definition(TYPE, NAME) _declspec ( property ( put = p_mutator_name(NAME)) ) TYPE NAME
#define p_mutable_definition(TYPE, NAME) _declspec ( property ( get = p_name(NAME), put = p_mutator_name(NAME)) ) TYPE NAME
#define p_virtual_definition(TYPE, NAME) _declspec ( property ( get = p_virtual_name(NAME)) ) TYPE NAME
#define p_virtual_mutator_definition(TYPE, NAME) _declspec ( property ( put = p_virtual_mutator_name(NAME)) ) TYPE NAME

#define p_function(TYPE, NAME) TYPE const p_name(NAME)()const
#define p_nonconst_function(TYPE, NAME) TYPE p_name(NAME)()
#define p_mutator_function(TYPE, NAME) void p_mutator_name(NAME)(TYPE const&value)

#define p_nonconst_implementation(CLASS, TYPE, NAME) TYPE CLASS::p_name(NAME)()
#define p_implementation(CLASS, TYPE, NAME) TYPE const CLASS::p_name(NAME)()const
#define p_mutator_implementation(CLASS, TYPE, NAME) void CLASS::p_mutator_name(NAME)(TYPE const&value)

#define p_nonconst(TYPE, NAME) p_definition(TYPE,NAME); p_nonconst_function(TYPE,NAME)
#define p(TYPE, NAME) p_definition(TYPE,NAME); p_function(TYPE,NAME)

#define p_mutator(TYPE, NAME) p_mutator_definition(TYPE,NAME); p_mutator_function(TYPE,NAME)
#define p_mutable(TYPE, NAME) p_mutable_definition(TYPE,NAME); p_mutator_function(TYPE,NAME); p_function(TYPE,NAME)
#define p_mutable_set(TYPE, NAME) p_mutable_definition(TYPE,NAME); p_function(TYPE,NAME); p_mutator_function(TYPE,NAME)


#define p_virtual_name(NAME) virtual_get_##NAME
#define p_virtual_mutator_name(NAME) virtual_set_##NAME

#define p_virtual_function(TYPE, NAME) TYPE const p_virtual_name(NAME)()const
#define p_virtual_mutator_function(TYPE, NAME) virtual void p_virtual_mutator_name(NAME)(TYPE const&value)

#define p_virtual(TYPE, NAME) p_virtual_definition(TYPE,NAME);\
    p_virtual_function(TYPE,NAME);\
    virtual p_function(TYPE,NAME)
#define p_virtual_inline(TYPE, NAME) p_virtual_definition(TYPE,NAME);\
    p_virtual_function(TYPE,NAME){ return p_name(NAME)(); };\
    virtual p_function(TYPE,NAME)

#define p_virtual_header_implementation_header(CLASS, TYPE, NAME) TYPE const CLASS::p_virtual_name(NAME)()const
#define p_virtual_header_implementation(CLASS, TYPE, NAME) p_virtual_header_implementation_header(CLASS, TYPE, NAME) { return p_name(NAME)(); };
#define p_virtual_mutator_header_implementation(CLASS, TYPE, NAME) void CLASS::p_virtual_mutator_name(NAME)(TYPE const&value){p_mutator_name(NAME)(value);}

#define p_virtual_implementation(CLASS, TYPE, NAME) \
    p_virtual_header_implementation(CLASS, TYPE, NAME);\
    p_implementation(CLASS, TYPE, NAME)

#define p_base_name(NAME) baseType::get_##NAME()

#define ThisRef p_nonconst(thisType&, thisRef)const{return const_cast<thisType&>(*this);}


#include "_EditorTemplates.h"
