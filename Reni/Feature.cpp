#include "Import.h"
#include "Feature.h"
#include "ContextFeature.h"
#include "DefineableToken .h"
#include "FeatureClass.h"
#include "ExpressionSyntax.h"
#include "Result.h"
#include "../HWLib/Ref.h"

static bool Trace = true;

using namespace HWLib;
using namespace Reni;


ResultData const Feature::FunctionResult(
    Context const&context,
    Category category,
    ExpressionSyntax const& expressionSyntax
)const{
    md(context, category, expressionSyntax);
    b_;
    return{};
};


ResultData const ContextFeature::FunctionResult(
    Context const&context,
    Category category,
    ExpressionSyntax const& expressionSyntax
)const{
    a_if(expressionSyntax.left.IsEmpty, nd(expressionSyntax.left));    
    return FunctionResult(context, category, expressionSyntax.right);
}

ResultData const ContextFeature::FunctionResult(
    Context const&context,
    Category category,
    Ref<Syntax, true> const& right
)const{
    md(context, category, right);
    b_;
    return{};
}


p_implementation(DefinableFeatureClass, Array<String>, DumpData){
    return{nd(parent)};
}

SearchResult const DefinableFeatureClass::GetDefinition(Type const&target)const{
    return target.GetDefinition(parent);
}

SearchResult const DefinableFeatureClass::GetDefinition(Context const&target)const{
    return target.GetDefinition(parent);
}

SearchResult const FeatureClass::GetDefinition(WeakPtr<Type> type, Context const&context)const
{
    if(type.IsEmpty)
    {
        bool Trace = false;
        md(context);
        return_d(GetDefinition(context));
    }
    else
    {
        bool Trace = type->ObjectId == 11;
        md(*type);
        b_if_(Trace);
        auto result = GetDefinition(*type);
        return_d(result);
    }
};
