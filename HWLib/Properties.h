#undef SET

#define GET_PROPERTY(TYPE, NAME) _declspec ( property ( get = get_##NAME) ) TYPE NAME
#define SET_PROPERTY(TYPE, NAME) _declspec ( property ( put = set_##NAME) ) TYPE NAME
#define GETSET_PROPERTY(TYPE, NAME) _declspec ( property ( get = get_##NAME, put = set_##NAME) ) TYPE NAME; 

#define GET_FUNCTION(TYPE, NAME) TYPE const get_##NAME()const
#define GET_FUNCTION_NONCONST(TYPE, NAME) TYPE get_##NAME()
#define SET_FUNCTION(TYPE, NAME) void set_##NAME(TYPE const&value)

#define GET_IMPLEMENTATION(CLASS, TYPE, NAME) TYPE const CLASS::get_##NAME()const
#define SET_IMPLEMENTATION(CLASS, TYPE, NAME) void CLASS::set_##NAME(TYPE const&value)

#define p(TYPE, NAME) GET_PROPERTY(TYPE,NAME); GET_FUNCTION(TYPE,NAME)
#define virtual_p(TYPE, NAME) GET_PROPERTY(TYPE,NAME); virtual GET_FUNCTION(TYPE,NAME)

#define p_mutable(TYPE, NAME) GETSET_PROPERTY(TYPE,NAME); SET_FUNCTION(TYPE,NAME); GET_FUNCTION(TYPE,NAME)
#define p_mutable_set(TYPE, NAME) GETSET_PROPERTY(TYPE,NAME); GET_FUNCTION(TYPE,NAME); SET_FUNCTION(TYPE,NAME)
