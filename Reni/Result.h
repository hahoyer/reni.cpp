#pragma once
#include "ResultData.h"
#include "../HWLib/Pod.h"

using namespace HWLib;
using namespace Util;
using namespace std;


namespace Reni
{
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
        pod<bool> Trace;

        ResultCache();

        ThisRef;
        p(bool, hllw);
        p(Size, size);
        p(Ref<CodeItem >, code);
        p(WeakRef<Type>, type);
        p(Externals, exts);
        p(Optional<WeakRef<Type>>, cachedType);

        ResultData const Get(Category category)const;
    private:
        void Ensure(Category category)const;
    protected:
        p_function(Array<String>,DumpData) override;
        virtual_p(bool, isRecursion) { return false; };
        virtual ResultData const GetResultData(Category category)const = 0;
        virtual ResultData const GetResultDataRecursive(Category category) const;
    private:
        p(Category, complete);
    };


    class Syntax;
    class Context;
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
        ResultData const GetResultDataRecursive(Category category) const override;
    };

}

