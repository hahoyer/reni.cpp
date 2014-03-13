#include "Import.h"
#include "Context.h"

#include "FeatureClass.h"
#include "Result.h"
#include "SearchResult.h"
#include "Syntax.h"
#include "Type.h"

using namespace Reni;
static bool Trace = true;


ResultData const Context::GetResultData(Category category, Syntax const&syntax)const{
    return syntax.GetResultData(*this, category);
}

pure_p_implementation(Context, WeakRef<RootContext>, rootContext) ;


SearchResult const Context::Search(Ref<Syntax, true> const&left, TokenClass const&tokenClass)const{
    if(left.IsEmpty)
        return tokenClass.featureClass->GetDefinition(*this);
    return tokenClass.featureClass->GetDefinition(*left->Type(*this));
}
