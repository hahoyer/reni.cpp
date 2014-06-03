#pragma once

#include "../HWLib/WeakRef.h"
#include "../HWLib/WeakPtr.h"
#include "../HWLib/Optional.h"
#include "../Util/Size.h"
#include "../HWLib/Ref.h"
#include "../Util/Category.h"
#include "../HWLib/RefCountProvider.h"
#include "../HWLib/CtrlRef.h"
#include "External.h"

using namespace HWLib;
using namespace Util;
using namespace std;


namespace Reni
{
    class CodeItem;
    class Context;
    class External;
    class ReplaceVisitor;
    class Syntax;
    class Type;

    class ResultData final : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = ResultData;
    public:
        Optional<Size> const size;
        WeakPtr<Type> const type;
        Ref<CodeItem, true> const code;
        Optional<Array<Ref<External>>>  const externals;

        ResultData() { SetDumpString(); };
        ResultData(Ref<CodeItem> code);
        ResultData(Type const& type);
        ResultData(Array<Ref<External>> externals);

    private:
        ResultData(Optional<Size> const&size, Ref<CodeItem, true> code, WeakPtr<Type> type, Optional<Array<Ref<External>>> const&externals)
            : size(size)
            , code(code)
            , type(type)
            , externals(externals)
        {
            SetDumpString();
            AssertValid();
        };

    public:
        static ResultData Get(
            Category category, 
            function<Ref<CodeItem>()> getCode, 
            function<WeakRef<Type>()> getType
            );
        static ResultData Get(
            Category category,
            CodeItem const& code,
            function<WeakRef<Type>()> getType
            );
        static ResultData Get(
            Category category,
            function<Ref<CodeItem>()> getCode,
            Type const& type
            );
        static ResultData Get(
            Category category,
            CodeItem const& code,
            Type const& type
            );



        DefaultAssignmentOperator;
        ResultData const operator+(ResultData const&other)const;
        ResultData const operator&(Category const&other)const;
        p(Category, complete){return Category::Instance(size.IsValid, !code.IsEmpty, !type.IsEmpty, externals.IsValid);}

        ResultData const With(CodeItem const& code) const;
        ResultData const With(Type const& type) const;
        ResultData const Replace(ReplaceVisitor const&arg) const;
    private:
        p_function(Array<String>,DumpData) override;
        void AssertValid();
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
        p(Array<Ref<External>>, externals);
        p(WeakPtr<Type>, cachedType);

        ResultData const Get(Category category)const;
    private:
        void Ensure(Category category)const;
    protected:
        p_function(Array<String>,DumpData) override;
        virtual_p(bool, isRecursion) { return false; };
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
        ResultFromSyntaxAndContext(Syntax const& syntax, Context const&context);

    private:
        p_function(Array<String>,DumpData) override;
        p_function(bool, isRecursion) override;
        ResultData const GetResultData(Category category)const override;
    };

}

