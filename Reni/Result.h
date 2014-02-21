#pragma once

#include "Category.h"
#include "Code.h"

namespace Reni
{
    class CodeItem;
    class Context;

    class ResultData final : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = ResultData;
    public:
        OptRef<CodeItem> code;

        DefaultAssignmentOperator;

        ResultData(Ref<CodeItem> code) : code(code){};
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
        mutable ResultData data;
        mutable Category pending;
    public:
        Result(Syntax const& syntax, Context const&context);
        p(Ref<CodeItem>, Code);
    private:
        override_p_function(Array<String>, DumpData);

        void Ensure(Category category)const;
        ResultData const GetResultData(Category category)const;
        p(Category, complete);
    };

}

