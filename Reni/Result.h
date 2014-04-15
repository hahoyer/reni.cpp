#pragma once

#include "../HWLib/WeakRef.h"
#include "../HWLib/WeakPtr.h"
#include "Type.h"
#include "Code.h"

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
        ResultData(Type const& type)
            : size(type.size)
            , type(type.thisRef){
            SetDumpString();
        };
        ResultData(Optional<Size> const&size, Ref<CodeItem,true> code, WeakPtr<Type> type)
            : size(size)
            , code(code)
            , type(type){
            SetDumpString();
        };

        DefaultAssignmentOperator;
        ResultData const operator+(ResultData const&other)const;
        ResultData const operator&(Category const&other)const;
        p(Category, complete){return Category::Instance(size.IsValid, !code.IsEmpty, !type.IsEmpty);}
        ResultData const With(CodeItem const& code) const{return ResultData(size, code.thisRef, type);}
        ResultData const With(Type const& type) const {return ResultData(size, code, type.thisRef);}
        ResultData const Replace(ReplaceVisitor const&arg) const;
    private:
        p_function(Array<String>,DumpData) override;
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
        ThisRef;
        p(Size, size);
        p(Ref<CodeItem >, code);
        p(WeakRef<Type>, type);

        ResultData const Get(Category category)const;
    private:
        void Ensure(Category category)const;
    protected:
        p_function(Array<String>,DumpData) override;
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
        p_function(Array<String>,DumpData) override;
        ResultData const GetResultData(Category category)const override;
    };

}

