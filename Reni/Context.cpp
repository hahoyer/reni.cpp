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

pure_p_implementation(Context, RootContext const&, rootContext);

SearchResult const Context::Search(OptRef<Syntax> const&left, TokenClass const&tokenClass)const
{
    typedef std::pair<WeakRef<FeatureClass>, WeakRef<DefinitionPoint>> pairType;
    if (left.IsValid)
    {
        auto fc = tokenClass.FeatureClasses;
        auto leftType = left->Type(*this);
        auto dp = leftType->DefinitionPoints;
        auto result = (fc * dp)
            ->Where([=](pairType const&pair)
        {
            return pair.second->Defines(*pair.first);
        })
            ->ToArray;

        switch (result.Count){
        case 0:
            return{};
        case 1:
            return result[0].second->Apply(*result[0].first);
        }
        md(left, tokenClass, fc, leftType, dp, result);
        b_;
        return{};
    }
    md(left, tokenClass);
    b_;
    return{};
}
