#define ccc(name, base)\
class name final : public base{\
    using baseType = base;\
    using thisType = name;\
}
