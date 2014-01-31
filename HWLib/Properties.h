#define p_definition(TYPE, NAME) _declspec ( property ( get = get_##NAME) ) TYPE NAME
#define p_mutator_definition(TYPE, NAME) _declspec ( property ( put = set_##NAME) ) TYPE NAME
#define p_mutable_definition(TYPE, NAME) _declspec ( property ( get = get_##NAME, put = set_##NAME) ) TYPE NAME

#define p_function(TYPE, NAME) TYPE const get_##NAME()const
#define mutable_p_function(TYPE, NAME) TYPE get_##NAME()
#define p_mutator_function(TYPE, NAME) void set_##NAME(TYPE const&value)

#define p_implementation(CLASS, TYPE, NAME) TYPE const CLASS::get_##NAME()const
#define p_mutator_imlementation(CLASS, TYPE, NAME) void CLASS::set_##NAME(TYPE const&value)

#define p(TYPE, NAME) p_definition(TYPE,NAME); p_function(TYPE,NAME)
#define virtual_p(TYPE, NAME) p_definition(TYPE,NAME); virtual p_function(TYPE,NAME)
#define virtual_mutable_p(TYPE, NAME) p_definition(TYPE,NAME); virtual mutable_p_function(TYPE,NAME)

#define p_mutable(TYPE, NAME) p_mutable_definition(TYPE,NAME); p_mutator_function(TYPE,NAME); p_function(TYPE,NAME)
#define p_mutable_set(TYPE, NAME) p_mutable_definition(TYPE,NAME); p_function(TYPE,NAME); p_mutator_function(TYPE,NAME)
