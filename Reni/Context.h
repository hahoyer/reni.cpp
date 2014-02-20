#pragma once

namespace Reni
{
    class Category;
    class ResultData;
    class Syntax;

    class Context : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = Context;
    public:
        Context();
        ResultData const GetResultData(Category category, Syntax const&syntax)const;
    };

    class RootContext : public Context
    {
        using baseType = Context;
        using thisType = RootContext;
    public:
        RootContext();
    private:
        override_p_function(Array<String>, DumpData);
    };

}

namespace std {
    template <>
    struct hash<Reni::Context const&>{
        inline std::size_t operator()(Reni::Context const& key) const{
            return key.ObjectId;
        }
    };
}
