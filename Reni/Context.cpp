#include "Import.h"
#include "Context.h"

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
    if (left.IsValid)
        return left->Type(*this)->Search(tokenClass);

    md(left, tokenClass);
    b_;
    return{};
}
