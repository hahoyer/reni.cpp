#pragma once
#include "Result.h"
using namespace HWLib;


namespace Reni
{
    class ResultDataDirect final : public ResultCache
    {
        typedef ResultCache baseType;
        typedef ResultFromSyntaxAndContext thisType;
        Optional<WeakRef<Type>> const type;
        Optional<Ref<CodeItem>> const code;
    public:

        ResultDataDirect(Optional<Ref<CodeItem>> code, Optional<WeakRef<Type>> type)
            : type(type)
            , code(code)
        {
            SetDumpString();
            if(!type.IsEmpty && code.IsValid)
                a_if(type.Value->size == code.Value->size, nd(code)+nd(type));
        }

        ResultDataDirect()
            : ResultDataDirect({}, {})
        {
        }

    private:
        p_function(Array<String>, DumpData) override
        {
            return{
                nd(code),
                nd(type)
            };
        };

        ResultData const GetResultData(Category category)const override
        {
            return ResultData::GetSmartHllwSizeExts(category, l_(code.Value), l_(type.Value));
        }
    };
}