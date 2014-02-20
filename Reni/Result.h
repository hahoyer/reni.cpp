#pragma once

namespace Reni
{
    class CodeItem;
    class Context;

    class Category final : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = Category;
    public:
        bool const hasSize;
        bool const hasCode;
    public:
        static Category const Code;
    private:
        Category(bool hasSize, bool hasCode);
    private:
        override_p_function(Array<String>, DumpData);
    };


    class Result final: public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = Result;

    public:
        Syntax const& syntax;
        Context const&context;
    private:
        class internal;
        Pointer<internal> cache;

    public:
        Result(Syntax const& syntax, Context const&context);
        p(Ref<CodeItem>, Code);
    private:
        override_p_function(Array<String>, DumpData);
    };

}

