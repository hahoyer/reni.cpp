#define ccc(name, base)\
class name final : public base{\
    typedef base baseType;\
    typedef name thisType;\
}
