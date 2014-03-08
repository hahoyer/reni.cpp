#pragma once
#include "../HWLib/RefCountProvider.h"
#include "../HWLib/DumpableObject.h"
#include "ExpressionSyntax.h"

using namespace HWLib;

namespace Reni{
    class ArrayType;


    class Feature
        : public DumpableObject
        , public RefCountProvider{
        typedef DumpableObject baseType;
        typedef Feature thisType;
    public:
        virtual ResultData const FunctionResult(
            Context const&context,
            Category category,
            ExpressionSyntax const& expressionSyntax
            )const;
    private:
        override_p_function(Array<String>, DumpData) {
            return{};
        }
    };

}
