#include "Import.h"
#include "SearchResult.h"

#include "Category.h"
#include "Context.h"
#include "ExpressionSyntax.h"
#include "Feature.h"
#include "FeatureProvider.h"

static bool Trace = true;

using namespace HWLib;
using namespace Reni;


SearchResult::SearchResult(Ref<Feature> feature) : feature(feature){
}

p_implementation(SearchResult, bool, IsValid){
    return feature.IsValid;
};

ResultData const SearchResult::FunctionResult(
    Context const&context, 
    Category category, 
    ExpressionSyntax const& expressionSyntax
    )const{
    md(context, category, expressionSyntax);
    b_;
    return{};
};

override_p_implementation(SearchResult, Array<String>, DumpData){
    return{nd(feature)};
}


#include "../HWLib/RefCountContainer.instance.h"
