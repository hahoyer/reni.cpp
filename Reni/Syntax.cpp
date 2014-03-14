#include "Import.h"
#include "Syntax.h"

#include "Category.h"
#include "Code.h"
#include "Context.h"
#include "FeatureProvider.h"
#include "Result.h"
#include "TokenClass.h"
#include "../HWLib/RefCountContainer.instance.h"

using namespace Reni;
static bool Trace = true;


Reni::Syntax::Syntax(SourcePart const&part)
    : part(part)
      , resultCache([&](Context const*context){
          return new ResultFromSyntaxAndContext(*this, *context);
      }){
};

Ref<CodeItem> const Syntax::Code(Context const&context)const{
    return resultCache[&context]->code;
}

WeakRef<Type>const Syntax::Type(Context const&context)const{
    return resultCache[&context]->type;
}

Ref<ResultFromSyntaxAndContext> const Syntax::GetResultCache(Context const&context)const {
    Ref<ResultFromSyntaxAndContext> r = resultCache[&context];
    return r;
}


ResultData const Syntax::GetResultData(Context const&context, Category category)const{
    md(context, category);
    b_;
    return_d(ResultData());
}

Ref<Syntax> const Syntax::SignMacro(TokenClass const&sign) const{
    md(sign);
    mb;
}

override_p_implementation(InfixSyntax, Array<String>, DumpData){
    return {
        nd(left),
        nd(tokenClass),
        nd(right)
    };
};


override_p_implementation(PrefixSyntax, Array<String>, DumpData){
    return {
        nd(tokenClass),
        nd(right)
    };
};


override_p_implementation(SuffixSyntax, Array<String>, DumpData){
    return {
        nd(left),
        nd(tokenClass)
    };
};