#pragma once

namespace Reni
{
    class Category;
    class ResultData;
    class Syntax;
    class RootContext;

    class Context : public WithId<DumpableObject, Context>
    {
        using baseType = WithId<DumpableObject, Context>;
        using thisType = Context;
    public:
        ResultData const GetResultData(Category category, Syntax const&syntax)const;
        virtual_p(RootContext const&, rootContext) = 0;
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
