#define p_name(NAME) get_##NAME
#define p_mutator_name(NAME) set_##NAME

#define p_definition(TYPE, NAME) _declspec ( property ( get = p_name(NAME)) ) TYPE NAME
#define p_mutator_definition(TYPE, NAME) _declspec ( property ( put = p_mutator_name(NAME)) ) TYPE NAME
#define p_mutable_definition(TYPE, NAME) _declspec ( property ( get = p_name(NAME), put = p_mutator_name(NAME)) ) TYPE NAME

#define p_function(TYPE, NAME) TYPE const p_name(NAME)()const
#define mutable_p_function(TYPE, NAME) TYPE p_name(NAME)()
#define p_mutator_function(TYPE, NAME) void p_mutator_name(NAME)(TYPE const&value)

#define mutable_p_implementation(CLASS, TYPE, NAME) TYPE CLASS::p_name(NAME)()
#define p_implementation(CLASS, TYPE, NAME) TYPE const CLASS::p_name(NAME)()const
#define p_mutator_implementation(CLASS, TYPE, NAME) void CLASS::p_mutator_name(NAME)(TYPE const&value)

#define mutable_p(TYPE, NAME) p_definition(TYPE,NAME); mutable_p_function(TYPE,NAME)
#define p(TYPE, NAME) p_definition(TYPE,NAME); p_function(TYPE,NAME)

#define p_mutator(TYPE, NAME) p_mutator_definition(TYPE,NAME); p_mutator_function(TYPE,NAME)
#define p_mutable(TYPE, NAME) p_mutable_definition(TYPE,NAME); p_mutator_function(TYPE,NAME); p_function(TYPE,NAME)
#define p_mutable_set(TYPE, NAME) p_mutable_definition(TYPE,NAME); p_function(TYPE,NAME); p_mutator_function(TYPE,NAME)


#define virtual_p_name(NAME) virtual_get_##NAME
#define virtual_p_mutator_name(NAME) virtual_set_##NAME

#define virtual_p_function(TYPE, NAME) virtual TYPE const virtual_p_name(NAME)()const
#define virtual_p_mutator_function(TYPE, NAME) virtual void virtual_p_mutator_name(NAME)(TYPE const&value)
#define override_p_function(TYPE, NAME) virtual TYPE const virtual_p_name(NAME)()const override
#define override_p_mutator_function(TYPE, NAME) virtual void virtual_p_mutator_name(NAME)(TYPE const&value)override

#define virtual_p(TYPE, NAME) p_definition(TYPE,NAME); p_function(TYPE,NAME); virtual_p_function(TYPE,NAME)
#define virtual_p_inline(TYPE, NAME) p_definition(TYPE,NAME); p_function(TYPE,NAME){ return virtual_p_name(NAME)(); }; virtual_p_function(TYPE,NAME)

#define virtual_mutable_p(TYPE, NAME) \
    p_mutable_definition(TYPE,NAME);\
    virtual_p_mutator_function(TYPE,NAME)\
    virtual_p_function(TYPE,NAME)

#define override_p_implementation(CLASS, TYPE, NAME) TYPE const CLASS::virtual_p_name(NAME)()const
#define override_p_mutator_implementation(CLASS, TYPE, NAME) void CLASS::virtual_p_mutator_name(NAME)(TYPE const&value)

#define pure_p_implementation(CLASS, TYPE, NAME) p_implementation(CLASS, TYPE, NAME){ return virtual_p_name(NAME)(); }; 
#define pure_p_mutator_implementation(CLASS, TYPE, NAME) p_mutator_implementation(CLASS, TYPE, NAME){virtual_p_mutator_name(NAME)(value);} 

#define virtual_p_implementation(CLASS, TYPE, NAME) \
    pure_p_implementation(CLASS, TYPE, NAME);\
    override_p_implementation(CLASS, TYPE, NAME)

#define virtual_p_mutator_implementation(CLASS, TYPE, NAME) \
    pure_p_mutator_implementation(CLASS,TYPE,NAME);\
    override_p_mutator_implementation(CLASS, TYPE, NAME) 

#define base_p_name(NAME) baseType::virtual_get_##NAME()

#define AssumeConstObject mutable_p(thisType&, ref)const{return const_cast<thisType&>(*this);}
