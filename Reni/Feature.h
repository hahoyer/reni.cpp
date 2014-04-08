#pragma once
#include "../HWLib/RefCountProvider.h"
#include "../HWLib/DumpableObject.h"
#include "../Util/Category.h"

using namespace HWLib;
using namespace Util;

namespace Reni{
    class ArrayType;
    class ExpressionSyntax;
    class ResultData;
    class Context;

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
        p_function(Array<String>,DumpData) override {
            return{};
        }
    };

}
                                                                    