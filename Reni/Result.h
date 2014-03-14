#pragma once

#include "Category.h"
#include "Type.h"
#include "Code.h"
#include "Size.h"
#include "../HWLib/WeakRef.h"
#include "../HWLib/WeakPtr.h"

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
        WeakPtr<Type> const type;
        Ref<CodeItem, true> const code;

        ResultData(){ SetDumpString(); };
        ResultData(Ref<CodeItem> code)
            : size(code->size)
            , code(code){
            SetDumpString();
        };
        ResultData(WeakRef<Type> type)
            : size(type->size)
            , type(type){
            SetDumpString();
        };
        ResultData(Optional<Size> const&size, Ref<CodeItem,true> code, WeakPtr<Type> type)
            : size(size)
            , code(code)
            , type(type){
            SetDumpString();
        };

        DefaultAssignmentOperator;

        p(Category, complete) 
        {
            return Category::Instance(size.IsValid, !code.IsEmpty, !type.IsEmpty);
        }
    private:
        override_p_function(Array<String>, DumpData);
    };


    class ResultCache 
        : public DumpableObject
        , public RefCountProvider
    {
        typedef DumpableObject baseType;
        typedef ResultCache thisType;
    protected:
        mutable ResultData data;
        mutable Category pending;
    public:
        AssumeConstObject;
        p(Size, size);
        p(Ref<CodeItem >, code);
        p(WeakRef<Type>, type);
    private:
        void Ensure(Category category)const;
    protected:
        override_p_function(Array<String>, DumpData);
        virtual ResultData const GetResultData(Category category)const = 0;
    private:
        p(Category, complete);
    };


    class ResultFromSyntaxAndContext final : public ResultCache
    {
        typedef ResultCache baseType;
        typedef ResultFromSyntaxAndContext thisType;
    public:
        Syntax const& syntax;
        Context const&context;
    public:
        ResultFromSyntaxAndContext(Syntax const& syntax, Context const&context);
    private:
        override_p_function(Array<String>, DumpData);
        virtual ResultData const GetResultData(Category category)const override;
    };

}

