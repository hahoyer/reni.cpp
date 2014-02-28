#pragma once

#include "Category.h"
#include "Type.h"
#include "Code.h"
#include "Size.h"

namespace Reni
{
    class CodeItem;
    class Context;
    class Syntax;

    class ResultData final : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = ResultData;
    public:
        Optional<Size> const size;
        OptRef<CodeItem> const code;
        OptRef<Type> const type;

        ResultData(){};
        ResultData(Ref<CodeItem> code) 
            : size(code->size)
            , code(code){};

        DefaultAssignmentOperator;

        p(Category, complete) 
        {
            return Category::Instance(size.IsValid, code.IsValid, type.IsValid);
        }
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
        p(Ref<CodeItem>, code);
        p(Ref<Type>, type);
    private:
        override_p_function(Array<String>, DumpData);

        void Ensure(Category category)const;
        ResultData const GetResultData(Category category)const;
        p(Category, complete);
    };

}

