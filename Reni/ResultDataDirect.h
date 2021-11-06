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
                HW_ASSERT(type.Value->size == code.Value->size, HW_D_VALUE(code)+HW_D_VALUE(type));
        }

        ResultDataDirect()
            : ResultDataDirect({}, {})
        {
        }

    private:
        p_function(Array<string>,DumpData) override
        {
            return{
                HW_D_VALUE(code),
                HW_D_VALUE(type)
            };
        };

        ResultData GetResultData(Category const&category)const override
        {
            return ResultData::GetSmartHollowSizeClosure(category, l_(code.Value), l_(type.Value));
        }
    };
}