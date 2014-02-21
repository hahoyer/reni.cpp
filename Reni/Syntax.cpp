#include "Import.h"
#include "Syntax.h"

#include "Category.h"
#include "Code.h"
#include "Context.h"
#include "Result.h"
//#include "Terminal.h"
#include "TokenClass.h"

using namespace Reni;
static bool Trace = true;

static int nextObjectId = 0;


Syntax::Syntax(SourcePart const&part)
: baseType(nextObjectId++)
, part(part)
, resultCache([&](Context const*context){return Result(*this, *context); }){};


Ref<CodeItem> Syntax::Code(Context const&context)const{
    return GetResult(context, Category::Code).Code;
}

ResultData const Syntax::GetResultData(Context const&context, Category category)const{
    md(context, category);
    b_;
    return_d(ResultData());
}


Result Syntax::GetResult(Context const&context, Category category)const{
    return resultCache[&context];
};


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
