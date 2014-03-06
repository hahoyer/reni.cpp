#pragma once

#include "Category.h"
#include "Type.h"
#include "Code.h"
#include "Size.h"
#include "../HWLib/WeakRef.h"
#include "../HWLib/OptWeakRef.h"

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
        CtrlPtr<CodeItem> const code;
        OptWeakRef<Type> const type;

        ResultData(){};
        ResultData(CtrlRef<CodeItem> code)
            : size(code->size)
            , code(code){};
        ResultData(WeakRef<Type> type)
            : size(type->size)
            , type(type){};
        ResultData(Optional<Size> const&size, CtrlPtr<CodeItem> code, OptWeakRef<Type> type)
            : size(size)
            , code(code)
            , type(type){};

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
        p(CtrlRef<CodeItem >, code);
        p(WeakRef<Type>, type);
    private:
        override_p_function(Array<String>, DumpData);

        void Ensure(Category category)const;
        ResultData const GetResultData(Category category)const;
        p(Category, complete);
    };

}

