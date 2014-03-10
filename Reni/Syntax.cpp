#include "Import.h"
#include "Syntax.h"

#include "Category.h"
#include "Code.h"
#include "Context.h"
#include "Result.h"
#include "TokenClass.h"

using namespace Reni;
static bool Trace = true;


Reni::Syntax::Syntax(SourcePart const&part)
    : part(part)
      , resultCache([&](Context const*context){
          return ResultFromSyntaxAndContext(*this, *context);
      }){
};

Ref<CodeItem> const Syntax::Code(Context const&context)const{
    return resultCache[&context].code;
}

WeakRef<Type>const Syntax::Type(Context const&context)const{
    return resultCache[&context].type;
}

WeakRef<ResultCache> const Syntax::GetResultCache(Context const&context)const{
    WeakRef<ResultCache> r = resultCache[&context].ref;
    return r;
}


ResultData const Syntax::GetResultData(Context const&context, Category category)const{
    md(context, category);
    b_;
    return_d(ResultData());
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