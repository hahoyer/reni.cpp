#pragma once

#include "../HWLib/WeakRef.h"
#include "../HWLib/WeakPtr.h"
#include "../HWLib/Optional.h"
#include "../Util/Size.h"
#include "../HWLib/Ref.h"
#include "../Util/Category.h"
#include "../HWLib/RefCountProvider.h"
#include "../HWLib/CtrlRef.h"
#include "Externals.h"

using namespace HWLib;
using namespace Util;
using namespace std;


namespace Reni
{
    class CodeItem;
    class Context;
    class ReplaceVisitor;
    class Syntax;
    class Type;

    class ResultData final : public DumpableObject
    {
        using baseType = DumpableObject;
        using thisType = ResultData;
    public:
        Optional<bool> const hllw;
        Optional<Size> const size;
        WeakPtr<Type> const type;
        Optional<Ref<CodeItem>> const code;
        Optional<Externals>  const exts;

        ResultData() { SetDumpString(); };
        ResultData(Ref<CodeItem> code);
        ResultData(Type const& type);
        ResultData(Externals exts);

    private:
        ResultData(
            Optional<bool> const&hllw,
            Optional<Size> const&size,
            Optional<Ref<CodeItem>> const&code, 
            WeakPtr<Type> type, 
            Optional<Externals> const&exts
            )
            : hllw(hllw)
            , size(size)
            , code(code)
            , type(type)
            , exts(exts)
        {
            SetDumpString();
            AssertValid();
        };

        static ResultData const FullGet(
            Category category,
            Optional<bool> const&hllw,
            Optional<Size> const&size,
            Optional<Ref<CodeItem>> const&code,
            WeakPtr<Type> type,
            Optional<Externals> const&exts
            )
        {
            AssertValid(category, hllw, size, code, type, exts) ;
            return ResultData(hllw, size, code, type, exts) & category;
        };
    public:
        static ResultData const Get(
            Category category,
            function<bool()> const&getHllw,
            function<Size()> getSize,
            function<Ref<CodeItem>()> getCode,
            function<WeakRef<Type>()> getType,
            function<Externals()> getExts
            );
        static ResultData const GetSmartHllwExts(
            Category category,
            function<Size()> getSize,
            function<Ref<CodeItem>()> getCode,
            function<WeakRef<Type>()> getType
            );
        static ResultData const GetSmartHllwSize(
            Category category,
            function<Ref<CodeItem>()> getCode,
            function<WeakRef<Type>()> getType,
            function<Externals()> getExts
            );
        static ResultData const GetSmartHllwSizeExts(
            Category category,
            function<Ref<CodeItem>()> getCode,
            function<WeakRef<Type>()> getType
            );

        DefaultAssignmentOperator;
        ResultData const operator+(ResultData const&other)const;
        ResultData const operator&(Category const&other)const;
        bool const operator==(thisType const&other)const;
        p(Category, complete){ return Category::Instance(hllw.IsValid, size.IsValid, !code.IsEmpty, !type.IsEmpty, exts.IsValid); }
        ResultData const Replace(ReplaceVisitor const&arg) const;
        ResultData const Convert(Type const& destination) const;
        p(ResultData, asFunctionResult);
    private:
        p_function(Array<String>,DumpData) override;
        void AssertValid();
        static Optional<bool>const ReplenishHllw(Category const&category, function<Ref<CodeItem>()> getCode, function<WeakRef<Type>()> getType);
        static Optional<Size>const ReplenishSize(Category const&category, function<Ref<CodeItem>()> getCode, function<WeakRef<Type>()> getType);
        static Optional<Externals> const ReplenishExternals(Category const&category, function<Ref<CodeItem>()> getCode);
        static void AssertValid(
            Category category,
            Optional<bool> const&hllw,
            Optional<Size> const size,
            Optional<Ref<CodeItem>> code,
            WeakPtr<Type> type,                            
            Optional<Externals> const&exts
            );
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
        p(bool, hllw);
        p(Size, size);
        p(Ref<CodeItem >, code);
        p(WeakRef<Type>, type);
        p(Externals, exts);
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

